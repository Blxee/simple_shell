#include "main.h"
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

/**
 * handle_path - prepends the correct directory
 *	in which the command was found
 *
 * @cmd: the command string to prepend path to (without args)
 * @envp: the environment variables vector
 *
 * Return:
 *	1 if the command was found
 *	0 elsewise
 */
int handle_path(char **cmd, char *envp[])
{
	char *path = NULL, *dir, cmd_cpy[128];

	_strcpy(cmd_cpy, *cmd); /* preserve the original command */
	if (access(cmd_cpy, F_OK) == 0) /* if cmd is a full path */
		return (1);
	while (*envp)
	{	/* look for PATH variable */
		if (_strncmp("PATH=", *envp, 5) == 0)
		{ /* PATH found */
			path = *envp + 5; /* +5 to jump the "PATH=" section */
			break;
		}
		envp++;
	}
	if (path && path[0] != '\0')
	{
		/* search PATH for file */
		dir = _strtok(path, ":");
		while (dir)
		{ /* go through each directory in PATH */
			*cmd = alloc_mem(_strlen(cmd_cpy) + _strlen(dir) + 2);
			_strcpy(*cmd, dir);
			_strcat(*cmd, "/");
			_strcat(*cmd, cmd_cpy);
			if (access(*cmd, F_OK) == 0) /* if file exists */
				return (1);
			dir = _strtok(NULL, ":");
		}
	}
	_strcpy(*cmd, cmd_cpy);
	return (0);
}

/**
 * child_process - creates a child process,
 *	then replaces it with the specified commmand
 *
 * @cmd: the command to run in this child process
 * @args: the command argument arrays (NULL terminated)
 */
void child_process(char *cmd, char *args[])
{
	if (execve(cmd, args, NULL) == -1)
	{ /* exec failed (cmd not found) */
		free_all();
		perror(*get_program_name());
		exit(127);
	}
}

/**
 * parse_cmd - splits command and its arguments
 *
 * @args: an array to store the arguments (starting with @cmd)
 * @line: the raw input line fromthe user
 * @envp: the environment variables vector
 * @stdin_fd: the input file director
 */
void parse_cmd(char *args[], char *line, char **envp, int stdin_fd)
{
	int i = 0;
	char *token, *quoted_strings[128], **quote = quoted_strings;

	replace_variables(&line, envp);
	get_quoted_strings(&line, quoted_strings, stdin_fd);
	token = _strtok(line, " \t\n");
	if (token)
	{
		expand_quote(&token, &quote);
		args[i++] = token;
	}
	while ((token = _strtok(NULL, " \t\n")))
	{
		expand_quote(&token, &quote);
		args[i++] = token;
	}
	args[i] = NULL; /*set the last element to NULL*/
}

/**
 * fork_process - forks the current process
 *
 * @is_interactive: boolean represents whether this is interactive session
 * @args: arguments to pass to execve syscall
 * @envp: the environment variables vector
 */
void fork_process(int is_interactive, char **args, char *envp[])
{
	static unsigned int prompt_number;
	int fork_ret, child_ret = 0;
	char **next_cmd = args, sep = ';';

	prompt_number++;
	while (sep && (sep == ';' || (sep == '&' && child_ret == 0)
				|| (sep == '|' && child_ret != 0)))
	{ /* for each (; || &&) separated command */
		args = next_cmd;
		next_separator(&next_cmd, &sep);
		if (!handle_path(&args[0], envp))
		{
			_writestr(STDERR_FILENO, *get_program_name());
			_writestr(STDERR_FILENO, ": ");
			_writestr(STDERR_FILENO, int_to_str(prompt_number));
			_writestr(STDERR_FILENO, ": ");
			_writestr(STDERR_FILENO, args[0]);
			_writestr(STDERR_FILENO, ": not found\n");
			*get_last_cmd_exit() = child_ret = 127;
			continue;
		}
		fork_ret = fork();
		if (fork_ret == -1)
		{ /* fork failed */
			perror(*get_program_name());
			free_all();
			exit(127);
		}
		if (fork_ret == 0) /* child process */
			child_process(args[0], args);
		else
		{ /* main process */
			wait(&child_ret); /* wait for the child process */
			child_ret = WEXITSTATUS(child_ret);
			*get_last_cmd_exit() = child_ret;
			free_mem(args[0]);
			if (!is_interactive && child_ret != 0)
				exit(child_ret);
		}
	}
}

/**
 * handle_exit - checks if a cmd is exit, and exit with status
 * @args: the command from the user and its arguments
 * Return: 1 if cmd is exit and successful, 0 otherwise.
 */
int handle_exit(char **args)
{
	char *status, *err_msg;
	int exit_status;
	size_t i;

	if (_strcmp(args[0], "exit") == 0)
	{
		status = args[1] ? args[1] : "0";
		for (i = 0; i < _strlen(status); i++)
		{
			if (!_isdigit(status[i]))
			{
				err_msg = "Invalide exit status: ";
				write(STDERR_FILENO, err_msg, _strlen(err_msg));
				write(STDOUT_FILENO, status, _strlen(status));
				write(STDOUT_FILENO, "\n", 1);
				return (1);
			}
		}
		exit_status = _atoi(status);
		exit(exit_status);
		return (1);
	}
	return (0);
}

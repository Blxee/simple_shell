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
 * @cmd: an array to store the command
 * @args: an array to store the arguments (starting with @cmd)
 * @line: the raw input line fromthe user
 */
void parse_cmd(char cmd[], char *args[], char *line)
{
	int i = 0, quote_idx;
	char *token, *quoted_strings[128], **quote = quoted_strings;

	get_quoted_strings(&line, quoted_strings);
	token = _strtok(line, " \t\n");
	if (token)
	{
		quote_idx = find_chars(token, "\"'");
		if (quote_idx != -1)
		{
			char *left_token = token, *right_token = token + quote_idx + 1;

			token[quote_idx] = '\0';
			token = alloc_mem(_strlen(left_token) + _strlen(*quote)
					+ _strlen(right_token) + 1);
			_strcpy(token, left_token);
			_strcat(token, *quote++);
			_strcat(token, right_token);
		}
		_strcpy(cmd, token); /* set cmd to be the first token */
		args[i++] = token;
	}
	while ((token = _strtok(NULL, " \t\n")))
	{
		quote_idx = find_chars(token, "\"'");
		if (quote_idx != -1)
		{
			char *left_token = token, *right_token = token + quote_idx + 1;

			token[quote_idx] = '\0';
			token = alloc_mem(_strlen(left_token) + _strlen(*quote)
					+ _strlen(right_token) + 1);
			_strcpy(token, left_token);
			_strcat(token, *quote++);
			_strcat(token, right_token);
		}
		args[i++] = token;
	}
	args[0] = cmd; /* set the first arg to the program name */
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
	int fork_ret, child_ret = 0;
	char **next_cmd = args, sep = ';';

	while (sep && (sep == ';' || (sep == '&' && child_ret == 0)
				|| (sep == '|' && child_ret != 0)))
	{ /* for each (; || &&) separated command */
		args = next_cmd;
		next_separator(&next_cmd, &sep);
		if (!handle_path(&args[0], envp))
		{
			perror(*get_program_name());
			child_ret = 1;
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

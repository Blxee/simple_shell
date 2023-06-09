#include "main.h"
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

/**
 * handle_path - prepends the correct directory
 *	in which the command was found
 *
 * @cmd: the command string to prepend path to (without args)
 *
 * Return:
 *	1 if the command was found
 *	0 elsewise
 */
int handle_path(char **cmd)
{
	char *path = NULL, *dir, cmd_cpy[128];
	unsigned int i = 0;

	_strcpy(cmd_cpy, *cmd); /* preserve the original command */
	if (access(cmd_cpy, F_OK) == 0) /* if cmd is a full path */
		return (1);
	while (environ[i])
	{	/* look for PATH variable */
		if (_strncmp("PATH=", environ[i], 5) == 0)
		{ /* PATH found */
			path = environ[i] + 5; /* +5 to jump the "PATH=" section */
			break;
		}
		i++;
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
	if (execve(cmd, args, environ) == -1)
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
 * @stdin_fd: the input file director
 */
void parse_cmd(char *args[], char *line, int stdin_fd)
{
	int i = 0, cmmt_idx;
	char *token = NULL, *quoted_strings[128], **quote = quoted_strings;

	cmmt_idx = find_chars(line, "#");
	if (cmmt_idx != -1 && line[cmmt_idx - 1] == ' ')
		line[cmmt_idx] = '\0';
	replace_variables(&line);
	get_quoted_strings(&line, quoted_strings, stdin_fd);
	token = strtok(line, " \t\n\r");
	if (token)
	{
		expand_quote(&token, &quote);
		args[i++] = token;
	}
	while ((token = strtok(NULL, " \t\n\r")))
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
 */
void fork_process(int is_interactive, char **args)
{
	static unsigned int prompt_number;
	int fork_ret, child_ret = 0;
	char **next_cmd = args, sep = ';';

	(void)is_interactive;
	prompt_number++;
	while (args[0] && sep && (sep == ';' || (sep == '&' && child_ret == 0)
				|| (sep == '|' && child_ret != 0)))
	{ /* for each (; || &&) separated command */
		args = next_cmd;
		next_separator(&next_cmd, &sep);
		/* replace_aliased(&args[0]); */
		if (check_custom_commands(args))
			continue;
		if (!handle_path(&args[0]))
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
			perror(*get_program_name()), free_all(), exit(127);
		if (fork_ret == 0) /* child process */
			child_process(args[0], args);
		else
		{ /* main process */
			wait(&child_ret); /* wait for the child process */
			child_ret = WEXITSTATUS(child_ret);
			*get_last_cmd_exit() = child_ret;
			/* free_mem(args[0]); */
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
char *status;
	int exit_status;
	unsigned int i;

	if (_strcmp(args[0], "exit") == 0)
	{
		if (args[1] == NULL)
		{
			free_all();
			exit(*get_last_cmd_exit());
			return (1);
		}
		status = args[1];
		if (status[0] == '-')
		{
			return (1);
		}
		else
		{
			for (i = 0; i < _strlen(status); i++)
			{
				if (!_isdigit(status[i]))
				{
					return (1);
				}
			}
			exit_status = _atoi(status);
		}
		free_all();
		exit(exit_status);
		return (1);
	}
	return (0);
}


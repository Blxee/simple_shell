#include "main.h"
#include <unistd.h>
#include <string.h>
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
int handle_path(char *cmd, char *envp[])
{
	char *path = NULL, *dir, cmd_cpy[128];

	_strcpy(cmd_cpy, cmd); /* preserve the original command */
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
		dir = strtok(path, ":");
		while (dir)
		{ /* go through each directory in PATH */
			_strcpy(cmd, dir);
			_strcat(cmd, "/");
			_strcat(cmd, cmd_cpy);
			if (access(cmd, F_OK) == 0) /* if file exists */
				return (1);
			dir = strtok(NULL, ":");
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
 * @program_name: the name of the main program (argv[0])
 */
void child_process(char *cmd, char *args[], char *program_name)
{
	if (execve(cmd, args, NULL) == -1)
	{ /* exec failed (cmd not found) */
		perror(program_name);
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
	unsigned int i = 0;
	char *token;

	token = strtok(line, " \t\n");

	if (token)
		_strcpy(cmd, token); /* set cmd to be the first token */

	while (token)
	{
		args[i++] = token;
		token = strtok(NULL, " \t\n");
	}

	args[0] = cmd; /* set the first arg to the program name */
	args[i] = NULL; /*set the last element to NULL*/
}

/**
 * fork_process - forks the current process
 *
 * @is_interactive: boolean represents whether this is interactive session
 * @args: arguments to pass to execve syscall
 * @line: the input line to be freed when errors happen
 * @program_name: the name of the current program
 */
void fork_process(
		int is_interactive,
		char *args[],
		char line[],
		char *program_name)
{
	int fork_ret, child_ret;

	fork_ret = fork();
	if (fork_ret == -1)
	{ /* fork failed */
		perror(program_name);
		exit(127);
	}
	if (fork_ret == 0) /* child process */
		child_process(args[0], args, program_name);
	else
	{ /* main process */
		wait(&child_ret); /* wait for the child process */
		free(line);
		if (!is_interactive && WEXITSTATUS(child_ret) != 0)
			exit(WEXITSTATUS(child_ret));
	}
}

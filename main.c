#include "main.h"
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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
	char *path = NULL, *dir, cmd_cpy[128] = {0};

	_strcat(cmd_cpy, cmd); /* preserve the original command */
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
			*cmd = '\0';
			_strcat(cmd, dir);
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
	{
		cmd[0] = '\0';
		_strcat(cmd, token); /* set cmd to be the first token */
	}

	while (token)
	{
		args[i] = token;
		i++;
		token = strtok(NULL, " \t\n");
	}

	args[0] = cmd; /* set the first arg to the program name */
	args[i] = NULL; /*set the last element to NULL*/
}
/**
 *user_input - processes the user input
 *@input: a pointer to of string
 *@exit_flag: exit flag
 */
void user_input(char *input, int *exit_flag)
{
	if (_strcmp(input, "exit") == 0)
		*exit_flag = 1;
}
/**
 * main - simple shell program
 *
 * @argc: argument count
 * @argv: argument vector
 * @envp: environment variables vector
 *
 * Return: 0 (on success), 127 (on failure)
 */
int main(int argc, char *argv[], char *envp[])
{
	int fork_ret, child_ret, exit_flag = 0;
	char *line, cmd[1024] = {0}, *args[128];
	size_t mem_len;
	int is_interactive = isatty(STDOUT_FILENO) && isatty(STDIN_FILENO);

	(void)argc;
	while (!exit_flag)
	{
		if (is_interactive)
			_writestr("$ ");
		line = NULL;
		getline(&line, &mem_len, stdin);
		if (line == NULL)
			break;
		else if (line[0] == '\n')
			continue;
		parse_cmd(cmd, args, line);
		user_input(cmd, &exit_flag);
		if (!handle_path(cmd, envp))
		{
			free(line);
			perror(argv[0]);
			continue;
		}
		fork_ret = fork();
		if (fork_ret == -1)
		{ /* fork failed */
			perror(argv[0]);
			exit(127);
		}
		if (fork_ret == 0) /* child process */
			child_process(args[0], args, argv[0]); /*pass the entire command line to*/
		else
		{ /* main process */
			wait(&child_ret); /* wait for the child process */
			free(line);
			if (!is_interactive && WEXITSTATUS(child_ret) != 0)
				exit(WEXITSTATUS(child_ret));
		}
	}
	return (0);
}

#include "main.h"
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

/**
 * child_process - creates a child process,
 *	then replaces it with the specified commmand
 *
 * @cmd: the command to run in this child process
 * @cmd_len: the length of the command
 * @program_name: the name of the main program (argv[0])
 */
void child_process(char *cmd, ssize_t cmd_len, char *program_name)
{
	char *args[] = {NULL, NULL};

	cmd[cmd_len - 1] = '\0'; /* remove the newline char from inp */
	args[0] = cmd;

	if (execve(cmd, args, NULL) == -1)
	{ /* exec failed (cmd not found) */
		perror(program_name);
		exit(127);
	}
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
	int fork_ret, child_ret;
	char *cmd = NULL;
	ssize_t cmd_len;
	size_t mem_len;
	int is_interactive = isatty(STDOUT_FILENO) && isatty(STDIN_FILENO);

	(void)argc;
	(void)envp;

	while (1)
	{
		if (is_interactive)
			_writestr("$ ");
		cmd_len = getline(&cmd, &mem_len, stdin);

		fork_ret = fork();
		if (fork_ret == -1)
		{ /* fork failed */
			perror(argv[0]);
			exit(127);
		}
		else if (fork_ret == 0)
		{ /* child process */
			if (cmd == NULL)
				break;
			child_process(cmd, cmd_len, argv[0]);
		}
		else
		{ /* main process */
			wait(&child_ret); /* wait for the child process */
			free(cmd);
			cmd = NULL;
			if (!is_interactive && WEXITSTATUS(child_ret) != 0)
				exit(WEXITSTATUS(child_ret));
		}
	}

	return (0);
}

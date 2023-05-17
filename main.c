#include "main.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

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
	char *line, cmd[1024], *args[128];
	size_t mem_len;
	ssize_t read_bytes;
	int is_interactive = isatty(STDOUT_FILENO) && isatty(STDIN_FILENO);

	(void)argc;
	while (1)
	{
		if (is_interactive)
			_writestr("$ ");
		line = NULL;
		read_bytes = _getline(&line, &mem_len, STDIN_FILENO);
		if (read_bytes == -1)
			break;
		else if (read_bytes == 0)
			continue;
		parse_cmd(cmd, args, line);
		if (check_exit(cmd))
			exit(0);
		if (check_env(cmd, envp))
			continue;
		if (!handle_path(cmd, envp))
		{
			free(line);
			perror(argv[0]);
			continue;
		}
		fork_process(is_interactive, args, line, argv[0]);
	}
	free(line);
	return (0);
}

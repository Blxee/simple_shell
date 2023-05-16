#include "main.h"
#include <unistd.h>
#include <stdlib.h>

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
	int is_interactive = isatty(STDOUT_FILENO) && isatty(STDIN_FILENO);

	(void)argc;
	while (1)
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
	return (0);
}

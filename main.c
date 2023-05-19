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
	char *line = NULL, cmd[1024], *args[128];
	size_t mem_len = 0;
	ssize_t read_bytes;
	int is_interactive = isatty(STDOUT_FILENO) && isatty(STDIN_FILENO);

	*get_program_name() = argv[0]; /* set the program name static variable */
	(void)argc;
	while (1)
	{
		if (is_interactive)
			_writestr("$ ");
		read_bytes = _getline(&line, &mem_len, STDIN_FILENO);
		if (read_bytes == -1)
			break;
		else if (read_bytes == 1)
			continue;
		parse_cmd(cmd, args, line);
		if (handle_exit(args))
			break;
		if (check_custom_commands(args, envp))
			continue;
		if (!handle_path(cmd, envp))
		{
			free_all();
			perror(argv[0]);
			continue;
		}
		fork_process(is_interactive, args, line);
	}
	free_all();
	return (0);
}

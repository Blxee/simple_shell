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
	char *line = NULL, *args[128];
	size_t mem_len = 0;
	ssize_t read_bytes;
	int is_interactive = isatty(STDOUT_FILENO) && isatty(STDIN_FILENO);
	int stdin_fd;

	*get_program_name() = argv[0]; /* set the program name static variable */
	stdin_fd = open_file(argc, argv);
	if (stdin_fd != -1)
		is_interactive = 0;
	else
		stdin_fd = STDIN_FILENO;
	while (1)
	{
		if (is_interactive)
			_writestr(STDOUT_FILENO, "$ ");
		read_bytes = _getline(&line, &mem_len, stdin_fd);
		if (read_bytes == -1)
			break;
		else if (read_bytes == 0 || read_bytes == 1)
		{
			if (is_interactive)
				continue;
			else
				break;
		}
		parse_cmd(args, line, envp, stdin_fd);
		if (handle_exit(args))
			break;
		fork_process(is_interactive, args, envp);
		free_mem(line);
	}
	free_all();
	return (*get_last_cmd_exit());
}

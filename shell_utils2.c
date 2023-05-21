#include "main.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * get_program_name - returns the program name (it has to be set by main())
 *
 * Return: the program name
 */
char **get_program_name(void)
{
	static char *program_name;

	return (&program_name);
}

/**
 * check_custom_commands - executes a custom command if it finds one
 *
 * @args: the cmd and ita arguments (specified by the user)
 * @envp: the environment variables vector
 *
 * Return: 1 if it found a custom command and 0 if not
 */
int check_custom_commands(char **args, char **envp)
{
	char *cmd = args[0];

	if (check_env(cmd, envp))
		return (1);
	if (check_setenv(args, envp))
		return (1);
	if (check_unsetenv(args, envp))
		return (1);
	if (check_cd(args, envp))
		return (1);
	return (0);
}

/**
 * get_quoted_strings - fetches any quoted strings in a line
 *
 * @line: the address of the line to search
 * @quoted_strings: an array to store pointer to quoted strings
 */
void get_quoted_strings(char **line, char **quoted_strings)
{
	char quote[2], *new_line;
	int quote_start, quote_end, search_mark = 0,
			is_interactive = isatty(STDOUT_FILENO) && isatty(STDIN_FILENO);
	ssize_t new_line_len;
	size_t new_line_mem;

	while ((quote_start = find_chars(*line + search_mark, "\"'")) != -1)
	{ /* while there is another quoted string */
		*quote = (*line + search_mark)[quote_start];

		while ((quote_end = find_chars((*line + search_mark) + quote_start + 1, quote)) == -1)
		{ /* while the quote is unterminated */
			char *old_line = *line;

			if (is_interactive)
				_writestr(STDOUT_FILENO, "> ");
			new_line_len = _getline(&new_line, &new_line_mem, STDIN_FILENO);
			if (new_line_len == -1)
			{
				perror(*get_program_name());
				exit(127);
			}
			*line = alloc_mem(_strlen(old_line) + new_line_len);
			_strcpy(*line, old_line);
			free_mem(old_line);
			_strcat(*line, new_line);
		}
		quote_end += quote_start + 1;
		/* quote has been terminated! */
		/* append quoted str to quoted_strings */
		*quoted_strings = alloc_mem(quote_end - quote_start);
		(*line + search_mark)[quote_end] = '\0';
		_strcpy(*quoted_strings, *line + search_mark + quote_start + 1);
		(*line + search_mark)[quote_end] = *quote;
		/* replace it with single quote */
		_strcpy(*line + search_mark + quote_start, *line + search_mark + quote_end);

		search_mark += quote_start + 1;
		quoted_strings++;
	}
}

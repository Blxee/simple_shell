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
	if (check_alias(args))
		return (1);
	return (0);
}

/**
 * get_quoted_strings - fetches any quoted strings in a line
 *
 * @line: the address of the line to search
 * @quoted_strings: an array to store pointer to quoted strings
 * @stdin_fd: the input file director
 */
void get_quoted_strings(char **line, char **quoted_strings, int stdin_fd)
{
	char quote[2], *new_line;
	int quote_start, quote_end, search_mark = 0,
			is_interactive = isatty(STDOUT_FILENO) && isatty(STDIN_FILENO);
	ssize_t new_line_len;
	size_t new_line_mem;

	while ((quote_start = find_chars(*line + search_mark, "\"'")) != -1)
	{ /* while there is another quoted string */
		*quote = (*line + search_mark)[quote_start];

		while ((quote_end =
					find_chars((*line + search_mark) + quote_start + 1, quote)) == -1)
		{ /* while the quote is unterminated */
			char *old_line = *line;

			if (is_interactive)
				_writestr(STDOUT_FILENO, "> ");
			new_line_len = _getline(&new_line, &new_line_mem, stdin_fd);
			if (new_line_len == -1)
			{
				perror(*get_program_name());
				free_all();
				exit(127);
			}
			*line = alloc_mem(_strlen(old_line) + new_line_len);
			_strcpy(*line, old_line);
			free_mem(old_line);
			_strcat(*line, new_line);
		}
		quote_end += quote_start + 1;
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

/**
 * next_separator - moves to next command after a separator if it exists
 *
 * @next_cmd: pointer to an array of cmd arguments
 * @sep: an address to store the separator (;, &&, ||) if it was found
 */
void next_separator(char ***next_cmd, char *sep)
{
	char *separators[] = {";", "&&", "||", NULL};
	int i;

	while (**next_cmd)
	{
		*sep = 0;
		for (i = 0; separators[i]; i++)
			if (_strcmp(**next_cmd, separators[i]) == 0)
			{
				*sep = separators[i][0];
				**next_cmd = NULL;
				break;
			}
		(*next_cmd)++;
		if (*sep)
			break;
	}
}

/**
 * expand_quote - replaces the quote mark with its corresponding string
 *
 * @str: the string where to replace
 * @quotes: the quotes array
 */
void expand_quote(char **str, char ***quotes)
{
	int quote_idx;

	while ((quote_idx = find_chars(*str, "\"'")) != -1)
	{
		char *left_token = *str, *right_token = *str + quote_idx + 1;

		*str[quote_idx] = '\0';
		*str = alloc_mem(
				_strlen(left_token)
				+ _strlen(**quotes)
				+ _strlen(right_token) + 1);
		_strcpy(*str, left_token);
		_strcat(*str, **quotes);
		_strcat(*str, right_token);
		(*quotes)++;
	}
}

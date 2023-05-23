#include "main.h"

/**
 * get_var_name - retrieves the variable name after $
 *
 * @str: the string where to look
 * @var: the buffer to put the name
 *
 * Return: the length of th variable name
 */
int get_var_name(const char *str, char *var)
{
	int i = 0;
	char chr;

	while ((chr = str[i]))
	{
		if (!(chr >= '0' && chr <= '9')
				&& !(chr >= 'a' && chr <= 'z')
				&& !(chr >= 'A' && chr <= 'Z')
				&& !(chr == '_'))
			break;
		var[i] = str[i];
		i++;
	}
	var[i] = '\0';
	return (i);
}

/**
 * get_last_cmd_status - returns a pointer to the last cmd status
 *
 * Return: the status of the last executed cmd
 */
int *get_last_cmd_status(void)
{
	static int status_s;

	return (&status_s);
}

/**
 * replace_variables - replaces all variable names (which start with $)
 * with thier corresponding value
 *
 * @line: a pointer to the line to replace its variables
 * @envp: environment variables vector
 */
void replace_variables(char **line, char **envp)
{
	int sign_indices[64], sign_idx, i = 0, j;
	unsigned int linelen = _strlen(*line), offset = 0;

	while ((sign_indices[i] = find_chars(*line, "$")) != -1)
		(*line)[sign_indices[i++]] = ' ';
	i = 0;
	while ((sign_idx = sign_indices[i]) != -1)
	{ /* while there are more '$' in the line */
		char _var[256], *var = _var, *value = NULL, *newline;

		sign_idx += offset;
		j = 0;
		if ((*line)[sign_idx + 1] == '$')
			var = int_to_str(getpid());
		else if ((*line)[sign_idx + 1] == '?')
			var = int_to_str(WEXITSTATUS(*get_last_cmd_status()));
		else if ((*line)[sign_idx + 1] == ' ' || (*line)[sign_idx + 1] == '\0')
			var[j++] = '$';
		else
		{ /* if var is not a special variable */
			j = get_var_name(*line + sign_idx + 1, var);
			value = j ? _getenv(var, envp) : NULL;
			if (value)
				_strcpy(var, value + j + 1); /* copy after NAME= part */
			else /* if the var is not defined in envp */
				var[0] = '\0';
			free_mem(value);
		}
		linelen += _strlen(var);
		newline = alloc_mem(linelen + 1);
		(*line)[sign_idx] = '\0';
		strcpy(newline, *line);
		strcat(newline, var);
		strcat(newline, *line + sign_idx + j + 1);
		offset += _strlen(var) - j - 1;
		free_mem(var);
		free_mem(*line);
		*line = newline;
		i++;
	}
}

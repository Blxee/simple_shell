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
 * get_last_cmd_exit - returns a pointer to the last cmd status
 *
 * Return: the status of the last executed cmd
 */
int *get_last_cmd_exit(void)
{
	static int s_exit_status;

	return (&s_exit_status);
}

/**
 * get_var_value - sets a variable to its corresponding value
 *
 * @line: a pointer to the line to replace its variables
 * @sign_idx: index of the vatiable in the line
 * @envp: environment variables vector
 * @var: a buffer to store the value
 *
 * Return: length of the variable
 */
int get_var_value(char *line, int sign_idx, char **envp, char **var)
{
	int i;
	char *value = NULL;

	i = 1;
	if ((line)[sign_idx + 1] == 255)
		(*var) = int_to_str(getpid());
	else if (line[sign_idx + 1] == '?')
		(*var) = int_to_str(*get_last_cmd_exit());
	else
	{ /* if (*var) is not a special (*var)iable */
		i = get_var_name(line + sign_idx + 1, (*var));
		if (i)
		{
			value = _getenv((*var), envp);
			if (value)
				_strcpy((*var), value + i + 1); /* copy after NAME= part */
			else /* if the (*var) is not defined in envp */
				(*var)[0] = '\0';
		}
		else /* there is no (*var) name */
			(*var)[0] = '$';
		free_mem(value);
	}
	return (i);
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
	int sign_indices[64], sign_idx, i = 0, j, offset = 0;
	unsigned int linelen = _strlen(*line);

	while ((sign_indices[i] = find_chars(*line, "$")) != -1)
	{
		if ((*line)[sign_indices[i] + 1] == '$')
			(*line)[sign_indices[i] + 1] = 255;
		(*line)[sign_indices[i++]] = ' ';
	}
	for (i = 0; (sign_idx = sign_indices[i]) != -1; i++)
	{ /* while there are more '$' in the line */
		char _var[256], *var = _var, *newline;

		sign_idx += offset;
		j = get_var_value(*line, sign_idx, envp, &var);

		linelen += _strlen(var);
		newline = alloc_mem(linelen + 1);
		(*line)[sign_idx] = '\0';
		_strcpy(newline, *line);
		_strcat(newline, var);
		_strcat(newline, *line + sign_idx + j + 1);
		offset += _strlen(var) - j - 1;
		free_mem(var);
		free_mem(*line);
		*line = newline;
	}
}

/**
 * open_file - tries to open file from arguments given to the program
 *
 * @argc: argument count
 * @argv: argument vector
 *
 * Return: the file discriptor, -1 if there was no file arguments
 */
int open_file(int argc, char **argv)
{
	if (argc >= 2)
	{
		int fd = open(argv[1], O_RDONLY);

		if (fd == -1)
		{
			perror(*get_program_name());
			exit(2);
		}
		return (fd);
	}
	return (-1);
}

#include "main.h"
#include <unistd.h>

/**
 * _writestr - prints a string to stdout
 *
 * @str: a string to print
 *
 * Return: amount chars printed, or -1 if an error occured
 */
int _writestr(char *str)
{
	unsigned int len = 0;

	while (str[len])
		len++;

	return (write(STDOUT_FILENO, str, len));
}

/**
 * _getline - reads an entire line from the input
 *
 * @line_p: a pointer to a pointer to character
 * @len_p: a pointer to an unsigned integer
 * @fd: a file descriptor
 *
 * Return: the total number of bytes read
 */
ssize_t _getline(char **line_p, size_t *len_p, int fd)
{
	ssize_t strlen = 0;
	char chr = 0, *new_p;

	if (*line_p == NULL || *len_p == 0)
	{
		*len_p = INIT_READ_SIZE;
		*line_p = malloc(*len_p * sizeof(char));
		if (*line_p == NULL)
			return (-1);
	}

	while (chr != '\n')
	{
		if (read(fd, &chr, 1) == -1)
			return (-1);

		if (strlen == (ssize_t)*len_p)
		{
			*len_p *= 2;
			new_p = malloc(*len_p * sizeof(char));
			if (new_p == NULL)
				return (-1);
			_strcpy(new_p, *line_p);
			free(*line_p);
			*line_p = new_p;
		}

		(*line_p)[strlen++] = chr;
	}

	(*line_p)[strlen] = '\0';

	return (strlen);
}

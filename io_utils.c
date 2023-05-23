#include "main.h"
#include <unistd.h>

/**
 * _writestr - prints a string to stdout
 *
 * @fd: a file durector to write to
 * @str: a string to print
 *
 * Return: amount chars printed, or -1 if an error occured
 */
int _writestr(int fd, char *str)
{
	unsigned int len = _strlen(str);

	return (write(fd, str, len));
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
	static char s_buffer[READ_BUFFER_SIZE];
	static int s_buflen, s_bufidx;
	ssize_t strlen = 0;
	char chr = 0, *new_p;

	if (*line_p == NULL || *len_p == 0)
	{
		*len_p = INIT_READ_SIZE;
		*line_p = alloc_mem(*len_p * sizeof(char));
		if (*line_p == NULL)
			return (-1);
	}
	while (chr != '\n')
	{
		if (s_bufidx >= s_buflen)
		{
			s_buflen = read(fd, s_buffer, READ_BUFFER_SIZE);
			if (s_buflen == -1)
				return (-1);
			s_bufidx = 0;
		}
		chr = s_buffer[s_bufidx++];
		if (strlen == (ssize_t)*len_p)
		{
			*len_p *= 2;
			new_p = alloc_mem(*len_p * sizeof(char));
			if (new_p == NULL)
				return (-1);
			_strcpy(new_p, *line_p);
			free_mem(*line_p);
			*line_p = new_p;
		}
		(*line_p)[strlen++] = chr;
		if ((!(isatty(STDOUT_FILENO) && isatty(STDIN_FILENO))
					|| fd != STDIN_FILENO)
				&& s_buflen < READ_BUFFER_SIZE
				&& s_buflen - s_bufidx == 1)
			break;
	}
	(*line_p)[strlen] = '\0';
	return (strlen);
}

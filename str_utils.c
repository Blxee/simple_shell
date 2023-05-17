#include "main.h"
#define READ_SIZE 1024


/**
 * _strlen - determines the length of a string
 *
 * @str: a string of chars
 *
 * Return: the length of @str
 */
unsigned int _strlen(char *str)
{
	unsigned int len = 0;

	while (str[len])
		len++;
	return (len);
}

/**
 * _strncmp - compares two strings
 *
 * @str1: the first string
 * @str2: the second string
 * @n: the amount of chars to be compared
 *
 * Return:
 *	0 if the s1 is equal to s2
 *	a negative number if s1 is less than s2
 *	a positive number if s1 is greater than s2
 */
int _strncmp(char *str1, char *str2, unsigned int n)
{
	while (*str1 && (*str1 == *str2) && n > 1)
	{
		str1++;
		str2++;
		n--;
	}

	return (*str1 - *str2);
}

/**
 * _strcat - concatenates one string to the other
 *
 * @dest: the destination to copy to
 * @src: the source to copy from
 *
 * Return: the @dest pointer
 */
char *_strcat(char *dest, const char *src)
{
	char *tmp = dest;

	if (tmp)
	{
		while (*tmp)
			tmp++;
		if (src)
			while (*src)
				*tmp++ = *src++;
		*tmp = '\0';
	}
	return (dest);
}

/**
 * _strcmp - Compares two strings
 *
 * @s1: pointer to a string
 * @s2: pointer to a string
 *
 * Return: an integer
 */
int _strcmp(char *s1, char *s2)
{
	int i;

	i = 0;
	while (s1[i] != '\0' && s2[i] != '\0')
	{
		if (s1[i] != s2[i])
		{
			return (s1[i] - s2[i]);
		}
		i++;
	}
	return (0);
}

/**
 * _strcpy - copies one string to the other
 *
 * @dest: the destination to copy to
 * @src: the source to copy from
 *
 * Return: the @dest pointer
 */
char *_strcpy(char *dest, const char *src)
{
	char *tmp = dest;

	if (tmp && src)
		do {
			*tmp++ = *src;
		} while (*src++);
	return (dest);
}
/**
 * _getline - reads an entire line from the input
 * @fd: a file descriptor
 * @lineptr: a pointer to a pointer to character
 * @n: a pointer to an unsigned integer
 * Return: the total number of bytes read
 */
ssize_t _getline(char **lineptr, size_t *n, int fd)
{
	static char buffer[READ_SIZE];
	static ssize_t i =0;
	static ssize_t buffer_size = 0;
	ssize_t sum_bytes_read = 0;
	ssize_t bytes_read = 0;
	char *tmp = NULL;

	if (!lineptr || !n)
		return (-1);
	if (!(*lineptr) || *n == 0)
	{
		*n = READ_SIZE;
		*lineptr = malloc(READ_SIZE);
		if (*lineptr == NULL)
			return (-1);
		*n = READ_SIZE;
	}
	tmp =*lineptr;
	while (1)
	{
		if (i >= buffer_size)
		{
			bytes_read = read(fd, buffer, READ_SIZE);
			if (bytes_read <= 0)
				break;
			i = 0; /* index of the buffer */
			buffer_size = bytes_read;
		}
		tmp[sum_bytes_read] = buffer[i];
		i++;
		sum_bytes_read++;
		if (tmp[sum_bytes_read - 1] == '\n')
			break;
		tmp++;
		if (sum_bytes_read >= (ssize_t)(*n))
			return (-1);
	}
	if (sum_bytes_read == 0)
	{
		free(*lineptr);
		*lineptr = NULL;
		*n = 0;
	}
	return (sum_bytes_read);
}

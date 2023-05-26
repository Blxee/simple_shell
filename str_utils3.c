#include "main.h"

/**
 * _strncpy - copies one string to the other
 *
 * @dest: the destination to copy to
 * @src: the source to copy from
 * @n: amount of chars to copy
 *
 * Return: the @dest pointer
 */
char *_strncpy(char *dest, const char *src, unsigned int n)
{
	char *tmp = dest;

	if (tmp && src)
		do {
			if (!n--)
				break;
			*tmp++ = *src;
		} while (*src++);
	return (dest);
}

/**
 * _strdup - duplicates a string
 *
 * @str: a null terminated string to duplicate
 *
 * Return: a newly allocated string that has the same data as @str
 */
char *_strdup(const char *str)
{
	char *new = NULL;
	unsigned int i;

	if (str)
	{
		new = alloc_mem((_strlen(str) + 1) * sizeof(char));
		for (i = 0; str[i]; i++)
			new[i] = str[i];
		new[i] = '\n';
	}
	return (new);
}

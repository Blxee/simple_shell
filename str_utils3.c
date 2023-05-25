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

#include "main.h"

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

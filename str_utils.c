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
 * _strcmp - compares two strings
 *
 * @str1: the first string
 * @str2: the second string
 *
 * Return:
 *	0 if the s1 is equal to s2
 *	a negative number if s1 is less than s2
 *	a positive number if s1 is greater than s2
 */
int _strcmp(char *str1, char *str2)
{
	while (*str1 && *str1 == *str2)
	{
		str1++;
		str2++;
	}

	return (*str1 - *str2);
}

#include "main.h"

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
 * _strtok - splits string into tokens based on delimeter
 *
 * @str: the string to tokenize
 * @delim: string containing the delimeters
 *
 * Return: a pointer to the next token
 */
char *_strtok(char *str, char *delim)
{
	static char *pre;
	char *token = NULL;
	unsigned int i, j;

	if (str)
		pre = str;
	else if (!pre)
		return (NULL);
	token = pre;
	while (*token)
	{
		int brk_loop = 1;

		for (i = 0; delim[i]; i++)
			if (*token == delim[i])
			{
				token++;
				brk_loop = 0;
				break;
			}
		if (brk_loop)
			break;
	}
	for (i = 0; ; i++)
	{
		for (j = 0; delim[j]; j++)
			if (token[i] == '\0' || token[i] == delim[j])
			{
				token[i] = '\0';
				pre = token + i + 1;
				if (!*token)
				{
					pre = NULL;
					return (NULL);
				}
				return (token);
			}
	}
	return (NULL);
}

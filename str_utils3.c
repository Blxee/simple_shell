#include "main.h"
/**
 * _strdup - Duplicates a string
 * @str: A pointer of character
 * Return: A pointer to a string
 */
char *_strdup(char *str)
{
	char *s;
	unsigned int i;

	if (str == NULL)
		return (NULL);
	s = malloc(sizeof(char) * (_strlen(str) + 1));
	if (s == NULL)
		return (0);
	for (i = 0; str[i] != '\0'; i++)
	{
		s[i] = str[i];
	}
	return (s);
}

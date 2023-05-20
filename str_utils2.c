#include "main.h"

/**
 * _strlen - determines the length of a string
 *
 * @str: the string which size is to be determined
 *
 * Return: the length of @str
 */
unsigned int _strlen(char *str)
{
	unsigned int len = 0;

	if (str)
		while (str[len])
			len++;
	return (len);
}

/**
 * _atoi - Converts a string to an integer.
 * @s: The pointer to a character
 * Return: An integer.
 */
int _atoi(char *s)
{
	int i = 0;
	unsigned int integer = 0;
	int sign = 1;
	int is_digit = 0;

	while (s[i] != '\0')
	{
		if (s[i] == 45)
		{
			sign = sign * (-1);
		}
		while (s[i] >= 48 && s[i] <= 57)
		{
			is_digit = 1;
			integer = (integer * 10) + (s[i] - '0');
			i++;
		}
		if (is_digit == 1)
		{
			break;
		}
		i++;
	}
	integer = integer * sign;
	return (integer);
}

/**
 * find_chars - searches for characters in a string
 *
 * @str: the string to search
 * @chars: characters to look for
 *
 * Return: the index of first occurence of any char
 */
int find_chars(char *str, char *chars)
{
	int i = 0, j;

  if (str && chars)
    while (str[i])
    {
      j = 0;
      while (chars[j])
      {
        if (str[i] == chars[j])
        	return (i);
        j++;
      }
      i++;
    }
  return (-1);
}

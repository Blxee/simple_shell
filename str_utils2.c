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

/**
 * _isdigit - check if a character is a digit
 * @c: char to be checked
 *
 * Return: 1 for a character that will ba a digit or 0 for any else
 */
int _isdigit(int c)
{
	if (c >= 48 && c <= 57)
	return (1);
	else
	return (0);
}

/**
 * int_to_str - converts integer to string
 *
 * @n: the integer to convert
 *
 * Return: a string containg @n (allocated)
 */
char *int_to_str(long n)
{
	char *str = alloc_mem(129 * sizeof(char));
	int len = 10, i = 0;

	if (n < 0)
	{
		n = -n;
		str[i++] = '-';
	}
	if (n < 10)
	{
		str[i++] = '0' + n;
		str[i] = '\0';
		return (str);
	}
	while (n / (len * 10) > 0)
		len *= 10;
	while (len > 0)
	{
		str[i++] = n / len % 10 + '0';
		len /= 10;
	}
	str[i] = '\0';
	return (str);
}

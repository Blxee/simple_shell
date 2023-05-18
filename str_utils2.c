#include "main.h"

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

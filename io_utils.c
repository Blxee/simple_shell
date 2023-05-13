#include "main.h"
#include <unistd.h>

/**
 * _writestr - prints a string to stdout
 *
 * @str: a string to print
 *
 * Return: amount chars printed, or -1 if an error occured
 */
int _writestr(char *str)
{
	unsigned int len = _strlen(str);

	return (write(STDOUT_FILENO, str, len));
}

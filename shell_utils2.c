#include "main.h"

/**
 * get_program_name - returns the program name (it has to be set by main())
 *
 * Return: the program name
 */
char **get_program_name(void)
{
	static char *program_name;

	return (&program_name);
}


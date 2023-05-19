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

/**
 * check_custom_commands - executes a custom command if it finds one
 *
 * @args: the cmd and ita arguments (specified by the user)
 * @envp: the environment variables vector
 *
 * Return: 1 if it found a custom command and 0 if not
 */
int check_custom_commands(char **args, char **envp)
{
	char *cmd = args[0];

	if (check_env(cmd, envp))
		return (1);
	if (check_setenv(args, envp))
		return (1);
	if (check_unsetenv(args, envp))
		return (1);
	return (0);
}

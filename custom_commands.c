#include "main.h"

/**
 * check_env - checks the "env" custom command
 *
 * @cmd: the command
 * @envp: environment variables array
 *
 * Return:
 *	1 if the command was "env"
 *	0 elsewise
 */
int check_env(char *cmd, char **envp)
{
	if (_strcmp(cmd, "env") == 0)
	{
		while (*envp)
		{
			_writestr(*envp);
			_writestr("\n");
			envp++;
		}
		return (1);
	}
	return (0);
}

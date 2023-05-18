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

/**
 * check_setenv - sets an environment variable if found
 *
 * @args: the command (of the user) and its arguments
 * @envp: the environment variable vector
 */
void check_setenv(char **args, char **envp)
{ /* NOTE: this function is still unfinished */
	unsigned int i = 0, varlen;
	char *cmd = args[0],
		*var = args[1],
		*value = args[2];
		/* *newvar; */

	if (cmd && _strcmp(cmd, "setenv") == 0)
	{ /* the command was setenv */
		if (var && value && !args[3])
		{ /* the arguments to setenv were correct */
			varlen = _strlen(var);
			var[varlen++] = '=';
			/* newvar = malloc((varlen + _strlen(value) + 2) * sizeof(char)); */
			while (envp[i])
			{ /* iterate through all environment variables */

				if (_strncmp(envp[i], var, varlen) != 0)
				{ /* the variable to be set exists */
					/* TODO: update the variable */
					return;
				}
				i++;
			}
			/* the variable does not exist, so it should be added */
			/* TODO: add the new variable */
		}
	}
}

/**
 * check_unsetenv - unsets an environment variable if found
 *
 * @args: the command (of the user) and its arguments
 * @envp: the environment variable vector
 */
void check_unsetenv(char **args, char **envp)
{
	unsigned int i = 0, varlen;
	char *cmd = args[0], *var = args[1];

	if (cmd && _strcmp(cmd, "unsetenv") == 0)
	{ /* the command was unsetenv */
		if (var && !args[2])
		{ /* the arguments to unsetenv were correct */
			varlen = _strlen(var);
			var[varlen++] = '=';
			while (envp[i])
			{ /* iterate through all environment variables */
				if (_strncmp(envp[i], var, varlen) != 0)
				{ /* the variable to be unset was found */
					do { /* offset all the next environment variables by 1 */
						envp[i] = envp[i + 1];
						i++;
					} while (envp[i]);
					return;
				}
				i++;
			}
		}
	}
}

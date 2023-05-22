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
			_writestr(STDOUT_FILENO, *envp);
			_writestr(STDOUT_FILENO, "\n");
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
 *
 * Return: whether the command was setenv
 */
int check_setenv(char **args, char **envp)
{
	unsigned int varlen;
	char *cmd = args[0], *var = args[1], *value = args[2], *newvar;

	if (cmd && _strcmp(cmd, "setenv") == 0)
	{ /* the command was setenv */
		if (var && value && !args[3])
		{ /* the arguments to setenv were correct */
			varlen = _strlen(var);
			newvar = alloc_mem((varlen + _strlen(value) + 2) * sizeof(char));
			_strcpy(newvar, var);
			_strcpy(newvar + varlen, "=");
			_strcpy(newvar + varlen + 1, value);
			var[varlen++] = '=';
			while (*envp)
			{ /* iterate through all environment variables */
				if (_strncmp(*envp, var, varlen) == 0)
				{ /* the variable to be set exists */
					free_mem(*envp);
					*envp = newvar;
					return (1);
				}
				envp++;
			}
			/* the variable does not exist, so it should be added */
			*envp++ = newvar;
			*envp = NULL;
			return (1);
		}
		else if (!args[1]) /* if setenv is without args just print the envp vector */
		{
			while (*envp)
			{
				_writestr(STDOUT_FILENO, *envp);
				_writestr(STDOUT_FILENO, "\n");
				envp++;
			}
			return (1);
		}
	}
	return (0);
}

/**
 * check_unsetenv - unsets an environment variable if found
 *
 * @args: the command (of the user) and its arguments
 * @envp: the environment variable vector
 *
 * Return: whether the command was unsetenv
 */
int check_unsetenv(char **args, char **envp)
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
				if (_strncmp(envp[i], var, varlen) == 0)
				{ /* the variable to be unset was found */
					do { /* offset all the next environment variables by 1 */
						envp[i] = envp[i + 1];
						i++;
					} while (envp[i]);
					return (1);
				}
				i++;
			}
		}
	}
	return (0);
}

/**
 * check_cd - checks whether the command is "cd", if so, it executes it
 *
 * @args: the command (of the user) and its arguments
 * @envp: the environment variable vector
 *
 * Return: whether the command was "cd"
 */
int check_cd(char **args, char **envp)
{
	char *dir = args[1], *oldpwd, *pwd, *oldpwd_cpy = NULL;
	char home_name[] = "HOME", oldpwd_name[] = "OLDPWD", pwd_name[] = "PWD";

	if (_strcmp(args[0], "cd") == 0)
	{
		oldpwd = _getenv(oldpwd_name, envp) + 7;
		if (!dir || !*dir || _strcmp(dir, "~") == 0)
			dir = _getenv(home_name, envp) + 5;
		else if (_strcmp(dir, "-") == 0)
		{
			oldpwd_cpy = alloc_mem((_strlen(oldpwd) + 1));
			dir = oldpwd;
			_strcpy(oldpwd_cpy, dir);
		}
		if (chdir(dir) == -1)
			perror(*get_program_name());
		pwd = _getenv(pwd_name, envp) + 4;
		_setenv(oldpwd_name, pwd, envp);
		_setenv(pwd_name, oldpwd_cpy ? oldpwd_cpy : dir, envp);
		free_mem(oldpwd_cpy);
		return (1);
	}
	return (0);
}

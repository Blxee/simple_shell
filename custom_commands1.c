#include "main.h"

/**
 * check_env - checks the "env" custom command
 *
 * @cmd: the command
 *
 * Return:
 *	1 if the command was "env"
 *	0 elsewise
 */
int check_env(char *cmd)
{
	unsigned int i = 0;

	if (_strcmp(cmd, "env") == 0)
	{
		while (environ[i])
		{
			_writestr(STDOUT_FILENO, environ[i]);
			_writestr(STDOUT_FILENO, "\n");
			i++;
		}
		*get_program_name() = 0;
		return (1);
	}
	return (0);
}

/**
 * check_setenv - sets an environment variable if found
 *
 * @args: the command (of the user) and its arguments
 *
 * Return: whether the command was setenv
 */
int check_setenv(char **args)
{
	char *cmd = args[0], *var = args[1], *value = args[2];
	unsigned int i = 0;

	if (cmd && _strcmp(cmd, "setenv") == 0)
	{ /* the command was setenv */
		if (var && value && !args[3])
		{ /* the arguments to setenv were correct */
			_setenv(var, value);
			return (1);
		}
		else if (!args[1])
		{ /* if setenv is without args just print the environ vector */
			while (environ[i])
			{
				_writestr(STDOUT_FILENO, environ[i]);
				_writestr(STDOUT_FILENO, "\n");
				i++;
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
 *
 * Return: whether the command was unsetenv
 */
int check_unsetenv(char **args)
{
	unsigned int i = 0, varlen;
	char *cmd = args[0], *var = args[1];

	if (cmd && _strcmp(cmd, "unsetenv") == 0)
	{ /* the command was unsetenv */
		if (var && !args[2])
		{ /* the arguments to unsetenv were correct */
			varlen = _strlen(var);
			var[varlen++] = '=';
			while (environ[i])
			{ /* iterate through all environment variables */
				if (_strncmp(environ[i], var, varlen) == 0)
				{ /* the variable to be unset was found */
					do { /* offset all the next environment variables by 1 */
						environ[i] = environ[i + 1];
						i++;
					} while (environ[i]);
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
 *
 * Return: whether the command was "cd"
 */
int check_cd(char **args)
{
	char *dir = args[1], *oldpwd, *pwd, *oldpwd_cpy = NULL;
	char home_name[] = "HOME", oldpwd_name[] = "OLDPWD", pwd_name[] = "PWD";

	if (_strcmp(args[0], "cd") == 0)
	{
		oldpwd = _getenv(oldpwd_name) + 7;
		if (!dir || !*dir || _strcmp(dir, "~") == 0)
			dir = _getenv(home_name) + 5;
		else if (_strcmp(dir, "-") == 0)
		{
			oldpwd_cpy = alloc_mem((_strlen(oldpwd) + 1));
			dir = oldpwd;
			_strcpy(oldpwd_cpy, dir);
		}
		if (chdir(dir) == -1)
			perror(*get_program_name());
		dir = alloc_mem(513 * sizeof(char));
		getcwd(dir, 512);
		pwd = _getenv(pwd_name) + 4;
		_setenv(oldpwd_name, pwd);
		_setenv(pwd_name, oldpwd_cpy ? oldpwd_cpy : dir);
		if (oldpwd_cpy)
		{
			_writestr(STDOUT_FILENO, oldpwd_cpy);
			_writestr(STDOUT_FILENO, "\n");
		}
		free_mem(oldpwd_cpy);
		return (1);
	}
	return (0);
}

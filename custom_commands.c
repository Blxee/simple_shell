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
					if (is_allocated(*envp))
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
 * _getenv - gets environment variable value
 * @var: the envirement variable name
 * @envp: a pointer to an array of the environment variables
 * Return: the value of the environment variable
 */
char *_getenv(char *var, char **envp)
{
	int i, var_len;
	
	var_len = _strlen(var);
	for (i = 0; envp[i] != NULL; i++)
	{
		if (_strncmp(envp[i], var, var_len) == 0)
			return (envp[i]);
	}
	return (NULL);
}
/**
 * get_cwd - gets the current working dir
 *
 * Return: 0 in success, -1 in failure
 */
char *get_cwd()
{
	static char cwd[256];

	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror("getcwd() error");
		return (NULL);
	}
	return (cwd);
}
/**
 * update_pwd - updates the environment variable PWD
 * @envp: a pointer to an array 
 * Return: 0 on success,-1 on failure
 */
int update_pwd(char **envp)
{
	char *cwd = get_cwd();
	char *pwd_var = _getenv("PWD", envp);
	if (!cwd)
		return (-1);
	if (!pwd_var)
	{
		perror("update_pwd error");
		return (-1);
	}
	_strcpy(pwd_var, cwd);return (0);
}
/**
 * check_cd - checks if the command is the built-in commad cd
 * @args: the command line array
 * @envp: the environment variable vector
 *
 * Return: an integer (0 on success, -1 on failure)
 */
int check_cd(char **args, char **envp)
{
	char *cmd = args[0], *directory = args[1];
	char *home, *oldpwd;

	if (cmd && _strcmp(cmd, "cd") == 0)
	{/* if the user entred the command cd*/
		if (!directory)
		{
			home = _getenv("HOME", envp);
			if (!home)
			{
				perror("cd");
				return (-1);
			}
			directory = home;
		}
		else if ( _strcmp(directory, "-") == 0)
		{
			oldpwd = _getenv("OLDPWD", envp);
			if (!oldpwd)
			{
				perror("OLDPWD error");
				return (-1);
			}
			directory = oldpwd;
			_writestr(STDOUT_FILENO, directory);
			_writestr(STDOUT_FILENO, "\n");
		}
		if (chdir(directory) != 0)
		{
			perror("cd");
			return (-1);
		}
		if (update_pwd(envp) != 0)
			return (-1);
	}
	return (0);
}

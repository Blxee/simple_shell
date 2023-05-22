#include "main.h"

/**
 * _getenv - retrieves an environment variable from envp
 *
 * @var: the name of the variable
 * @envp: the environment variable vector
 *
 * Return: the variable if it was found, NULL if not
 */
char *_getenv(char *var, char **envp)
{
	unsigned int varlen, i = 0;

	if (var && envp)
	{
		varlen = _strlen(var) + 1;
		while (var[i])
			i++;
		var[i] = '=';
		while (*envp)
		{ /* iterate through all environment variables */
			if (_strncmp(*envp, var, varlen) == 0)
			{
				var[i] = '\0';
				return (*envp);
			}
			envp++;
		}
	}
	var[i] = '\0';
	return (NULL);
}

/**
 * _setenv - sets an environment variable to a new value
 *
 * @var: the name of the variable to be set (ex: HOME)
 * @value: the new value
 * @envp: the environment variable vector
 *
 * Return: a pointer to the new variable
 */
char *_setenv(char *var, char *value, char **envp)
{
	unsigned int varlen = _strlen(var) + 1, i = 0, valuelen;

	valuelen = varlen + _strlen(value);
	while (var[i])
		i++;
	var[i] = '=';
	if (var && value && envp)
		while (*envp)
		{ /* iterate through all environment variables */
			if (_strncmp(*envp, var, varlen) == 0)
			{
				if (_strlen(*envp) < valuelen)
				{ /* if memory of *envp is not enough to hold new value */
					free_mem(*envp);
					*envp = alloc_mem(valuelen * sizeof(char));
				}
				var[i] = '\0';
				_strcpy(*envp, var);
				_strcat(*envp, "=");
				_strcat(*envp, value);
				return (*envp);
			}
			envp++;
		}
	var[i] = '\0';
	*envp = alloc_mem(valuelen * sizeof(char));
	_strcpy(*envp, var);
	_strcat(*envp, "=");
	_strcat(*envp, value);
	*(envp + 1) = NULL;
	return (*envp);
}

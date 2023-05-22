#include "main.h"

/**
 * _getenv - retrieves an environment variable from envp
 *
 * @var: the name of the variable
 * @envp: the environment variable vector
 *
 * Return: the variable if it was found, NULL if not
 */
char **_getenv(char *var, char **envp)
{
	unsigned int varlen = _strlen(var) + 1, i = 0;

	while (var[i])
		i++;
	var[i] = '=';
	if (var && envp)
		while (*envp)
		{ /* iterate through all environment variables */
			if (_strncmp(*envp, var, varlen) == 0)
				return (&*envp);
			envp++;
		}
	return (NULL);
}

/**
 * _setenv - sets an environment variable to a new value
 *
 * @var: the name of the variable to be set (ex: HOME)
 * @strarr: an array of string to be set and concatenated to the variable
 * @envp: the environment variable vector
 *
 * Return: the variable was found, NULL if not
 */
char *_setenv(char *var, char **strarr, char **envp)
{
	unsigned int varlen = _strlen(var) + 1, i, j, strarr_len;

	while (var[i])
		i++;
	var[i] = '=';
	if (var && strarr && envp)
		while (*envp)
		{ /* iterate through all environment variables */
			if (_strncmp(*envp, var, varlen) == 0)
			{
				strarr_len = varlen;
				for (j = 0; strarr[j]; j++)
					strarr_len += _strlen(strarr[j]);
				if (_strlen(*envp) < strarr_len)
				{ /* if memory of *envp is not enough to hold new value */
					free_mem(*envp);
					*envp = alloc_mem(strarr_len * sizeof(char));
				}
				var[i] = '\0';
				_strcpy(*envp, var);
				_strcat(*envp, "=");
				for (j = 0; strarr[j]; j++)
					_strcat(*envp, strarr[j]);
				return (*envp);
			}
			envp++;
		}
	var[i] = '\0';
	return (NULL);
}

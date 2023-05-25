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
				_strcpy(*envp, var), _strcat(*envp, "="), _strcat(*envp, value);
				return (*envp);
			}
			envp++;
		}
	var[i] = '\0';
	*envp = alloc_mem(valuelen + 1);
	_strcpy(*envp, var), _strcat(*envp, "="), _strcat(*envp, value);
	*(envp + 1) = NULL;
	return (*envp);
}

#ifdef ALIASES_SIZE
#undef ALIASES_SIZE
#endif /* !ALIASES_SIZE */
#define ALIASES_SIZE 128

/**
 * get_aliases - returns a pointer to the aliases array
 *
 * Return: the pointer to the aliases
 */
char **get_aliases(void)
{
	static char **s_aliases;

	if (s_aliases == NULL)
		s_aliases = alloc_mem(ALIASES_SIZE * 2 * sizeof(char));
	return (s_aliases);
}

/**
 * check_alias - checks whether the cmd is 'alias'
 *
 * @args: the cmd and its arguments
 *
 * Return: 1 if the cmd was 'alias', 0 elsewise
 */
int check_alias(char **args)
{
	char **aliases = get_aliases();
	char **next_alias = args + 1, *alias_name, *alias_value;
	unsigned int i;

	if (_strcmp(args[0], "alias") == 0)
	{
		if (*next_alias == NULL)
		{
			for (i = 0; aliases[i]; i += 2)
			{
				_writestr(STDOUT_FILENO, aliases[i + 0]);
				_writestr(STDOUT_FILENO, "=");
				_writestr(STDOUT_FILENO, aliases[i + 1]);
				_writestr(STDOUT_FILENO, "\n");
			}
			return (1);
		}
		while (*next_alias)
		{
			while (*aliases)
				aliases += 2;
			alias_name = _strtok(*next_alias, "=");
			alias_value = _strtok(NULL, "=");
			if (!alias_value)
			{
				_writestr(STDERR_FILENO, "Error: no value specified!\n");
				*get_last_cmd_exit() = 1;
				break;
			}
			aliases[0] = alloc_mem(_strlen(alias_name) + 1);
			aliases[1] = alloc_mem(_strlen(alias_value) + 1);
			_strcpy(aliases[0], alias_name);
			_strcpy(aliases[1], alias_value);
			next_alias++;
			aliases += 2;
		}
		return (1);
	}
	return (0);
}

/**
 * replace_aliased - replaces command if it's an alias
 *
 * @cmd: a pointer to the command
 */
void replace_aliased(char **cmd)
{
	char **aliases = get_aliases();

	while (*aliases)
	{
		char *alias = aliases[0], *value = aliases[1];

		if (_strcmp(*cmd, alias) == 0)
		{
			if (_strlen(value) > _strlen(*cmd))
				*cmd = alloc_mem(_strlen(value) + 1);
			_strcpy(*cmd, value);
		}
		aliases += 2;
	}
}

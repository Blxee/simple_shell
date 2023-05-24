#include "main.h"

/**
 * check_alias - checks if the command is 'alias'
 * @args: the command and its arguments 
 * @envp: the environment variables vector
 *
 * Return: an integer
 */
int check_alias(char *args[])
{
	if (_strcmp(cmd, "alias") == 0)
	{
		set_alias(args);
		return (1);
	}
	return (0);
}
/**
 * set_alias
 * @args:
 */
void set_alias(char **args)
{
	if (!arg[1])/* no given arguments, print aliases */
		print_aliases();
	else
	{
		for (i = 1; args[i] != NULL; i++)
		{
			name = _strtok(args[i], "=");
			value = _strtok(NULL, "=");
			if (!value)
			{
				for (j = 0; j < MAX_ALIASES; j++)
				{
					if (aliases[j] && _strcmp(aliases[j]->name, name) == 0)
					{
						_writestr(STDOUT_FILENO, aliases[j]->name);
						_writestr(STDOUT_FILENO, "=");
						_writestr(STDOUT_FILENO, aliases[j]->value);
						_writestr(STDOUT_FILENO, "\n");
						break;
					}
				}
			}
			else
			{
				Alias *alias = create_alias(name, value);
				added = 0;
				for (j = 0; j < MAX_ALIASES; j++)
				{
					if (aliases[j] && _strcmp(aliases[j]->name, name) == 0)
					{
						free(aliases[j]->value;
								aliases[j]->value = strdup(value);
								added = 1;
								break;
					}
				}
				if (!added)
				{
					free(alias->name);
					free(aliase_value);
					free(alias);
				}
			}
		}
	}
}
/**
 * create_alias
 * @name:
 * @value
 * Return: an alias
 */
Alias *create_alias(const char *name, const char *value)
{
	Alias *alias = malloc(sizeof(Alias));
	alias->name = strdup(name);
	alias->value = strdup(value);
	return (alias);
}
/**
 * print_aliases
 */
void print_aliases()
{
	int i;

	for (i = 0; i < MAX_ALIASES; i++)
	{
		if (aliases[i])
		{
			_writestr(STDOUT_FILENO, aliases[i]->name);
			_writestr(STDOUT	
		}
	}
}

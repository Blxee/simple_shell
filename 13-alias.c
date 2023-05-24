#include "main.h"
#include <string.h>
/**
 * check_alias - checks if the command is 'alias'
 * @args: the command and its arguments 
 * @envp: the environment variables vector
 *
 * Return: an integer
 */
int check_alias(char *args[])
{
	char *cmd = args[0];

	if (_strcmp(cmd, "alias") == 0)
	{
		set_alias(args);
		return (1);
	}
	return (0);
}
/**
 * set_alias
 * @args: the command and its arguments
 */
void set_alias(char **args)
{
	int i, j, added;
	char *name, *value;
	Alias *aliases[MAX_ALIASES] = {NULL}, *alias = NULL;

	if (!args[1])/* no given arguments, print aliases */
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
				alias = create_alias(name, value);
				added = 0;
				for (j = 0; j < MAX_ALIASES; j++)
				{
					if (aliases[j] && _strcmp(aliases[j]->name, name) == 0)
					{
						free_mem(aliases[j]->value);
						aliases[j]->value = strdup(value);
						added = 1;
						break;
					}
				}
				if (!added)
				{
					free_mem(alias->name);
					free_mem(alias->value);
					free_mem(alias);
				}
			}
		}
	}
}
/**
 * create_alias - creates a new alias if it doesn't exist
 * @name: name
 * @value: value
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
 * print_aliases - prints aliases
 */
void print_aliases()
{
	int i;
	Alias *aliases[MAX_ALIASES] = {NULL};

	for (i = 0; i < MAX_ALIASES; i++)
	{
		if (aliases[i])
		{
			_writestr(STDOUT_FILENO, aliases[i]->name);
			_writestr(STDOUT_FILENO, "=");
			_writestr(STDOUT_FILENO, aliases[i]->value);
			_writestr(STDOUT_FILENO, "\n");	
		}
	}
}

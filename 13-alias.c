#include "main.h"
#include <string.h>

/**
 * get_aliases - return the aliases global variable
 *
 * Return: aliases
 */
Alias **get_aliases(void)
{
	static Alias *aliases[MAX_ALIASES] = {NULL};

	return (aliases);
}

/**
 * check_alias - checks if the command is 'alias'
 * @args: the command and its arguments
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
 * set_alias - sets a new alias
 * @args: the command and its arguments
 */
void set_alias(char **args)
{
	int i, j, added, fd = STDOUT_FILENO;
	char *name, *value;
	Alias *alias = NULL, **aliases = get_aliases();

	if (!args[0])
		print_aliases();
	else
	{
		for (i = 0; args[i] != NULL; i++)
		{
			name = _strtok(args[i], "="), value = _strtok(NULL, "=");
			if (!value)
				for (j = 0; j < MAX_ALIASES; j++)
				{
					if (aliases[j] && _strcmp(aliases[j]->name, name) == 0)
					{
						_writestr(fd, aliases[j]->name), _writestr(fd, "="),
						_writestr(fd, aliases[j]->value), _writestr(fd, "\n");
						break;
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
						aliases[j]->value = _strdup(value);
						added = 1;
						break;
					}
				}
				if (!added)
					free_mem(alias->name), free_mem(alias->value), free_mem(alias);
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
Alias *create_alias(char *name, char *value)
{
	Alias *alias = malloc(sizeof(Alias));

	alias->name = _strdup(name);
	alias->value = _strdup(value);
	return (alias);
}
/**
 * print_aliases - prints aliases
 */
void print_aliases(void)
{
	int i;
	Alias **aliases = get_aliases();

	for (i = 0; i < MAX_ALIASES; i++)
	{
		if (aliases[i])
		{
			printf("%s='%s'\n", aliases[i]->name, aliases[i]->value);
		}
	}
}

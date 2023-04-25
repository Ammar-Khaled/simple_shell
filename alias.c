#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "builtin.h"


/**
 * print_alias_list - traverse over the global list
 * of aliases printing them in the form name='value'
 * @alias_list: head of alias list
 */
void print_alias_list(Alias *alias_list)
{
	Alias *cur_node = alias_list;

	while (cur_node)
	{
		printf("%s='%s'\n", cur_node->alias_name, cur_node->command);
		cur_node = cur_node->next;
	}
}

/**
 * lookup_alias - searches for an alias in the alias list
 * @name: alias_name
 * @alias_list: head of alias list
 * Return: the corresponding command for the passed alias name
 * or NULL if the alias is not found
 */
char *lookup_alias(char *name, Alias *alias_list)
{
	Alias *cur_node = alias_list;

	while (cur_node)
	{
		if (strcmp(cur_node->alias_name, name) == 0)
		{
			return (cur_node->command);
		}
		cur_node = cur_node->next;
	}

	return (NULL);
}

/**
 * set_alias - Defines an alias for `name` whose value is given.
 * If name is already an alias, replaces its value with `command`
 * @name: alias name
 * @command: corresponding command
 * @alias_list: head of alias list
 */
void set_alias(char *name, char *command, Alias *alias_list)
{
	Alias *cur_node, *new;

	cur_node = alias_list;
	if (!lookup_alias(name, alias_list))
	{
		/* add new alias at the beginning of the list*/
		new = malloc(sizeof(Alias));
		if (!new)
			return;
		strcpy(new->alias_name, name);
		strcpy(new->command, command);
		new->next = alias_list;
		alias_list = new;
	}
	else
	{
		/* set new value for existing alias */
		while (cur_node)
		{
			if (strcmp(cur_node->alias_name, name) == 0)
			{
				strcpy(cur_node->command, command);
				break;
			}
		}
	}
}

/**
 * builtin_alias - print or define aliases for commands
 * @argc: number of arguments
 * @argv: arguments list
 *
 * Return: exit status
 */
int builtin_alias(int argc, char **argv)
{
	int i, j, new_alias;
	char *name, *cmd;
	/* Define a pointer to the head of the alias linked list */
	static Alias *alias_list;

	if (argc == 1)
	{
		print_alias_list(alias_list);
	}
	else
	{
		/*for each given pair name='value'*/
		for (j = 1; argv[j]; j++)
		{
			for (i = 0, new_alias = 0; argv[j][i]; i++)
			{
				if (argv[j][i] == '=')
				{
					new_alias = 1;
					name = strtok(argv[j], "=");
					cmd = strtok(NULL, "");
					set_alias(name, cmd, alias_list);
				}
			}
			if (!new_alias)
			{
				/* print this alias*/
				if (!lookup_alias(argv[j], alias_list))
					printf("alias: %s: not found\n", argv[j]);
				else
					printf("%s=%s\n", argv[j], lookup_alias(argv[j], alias_list));
			}
		}

	}
	return (0);
}

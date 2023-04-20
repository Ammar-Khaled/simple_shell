#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
 * Alias - struct for an alias
 * @alias: the alias name
 * @command: the actual corresponding command
 */
typedef struct alias
{
	char *alias_name;
	char *command;
	struct alias *next;
} Alias;

/* Define a global pointer to the head of the alias linked list */
Alias *alias_list = NULL;

/**
 * print_alias_list - traverse over the global list
 * of aliases printing them in the form name='value'
 */
void print_alias_list(void)
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
 *
 * Return: the corresponding command for the passed alias name
 * or NULL if the alias is not found
 */
char *lookup_alias(char *name)
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
 */
void set_alias(char *name, char *command)
{
	Alias *cur_node, *new;

	cur_node = alias_list;
	if (!lookup_alias(name))
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

	if (argc == 1)
	{
		print_alias_list();
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
					set_alias(name, cmd);
				}
			}
			if (!new_alias)
			{
				/* print this alias*/
				if (!lookup_alias(argv[j]))
					printf("alias: %s: not found\n", argv[j]);
				else
					printf("%s=%s\n", argv[j], lookup_alias(argv[j]));
			}
		}

	}
	return (0);
}

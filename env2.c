#include "includes/env.h"
#include "includes/envar.h"
#include "includes/utils.h"
#include <stdlib.h>

/**
 * envman_search - search the environment variables
 * for variable with the same name
 * @name: the variable name to look for
 *
 * Return: the variable object otherwise NULL
 */
envar envman_search(char *name)
{
	envar var = envman_loop(1);

	while (var)
	{
		if (!_strcmp(name, var->name))
			return (var);
		var = envman_loop(0);
	}
	return (NULL);
}

/**
 * envman_value - gets the variable value if exists
 * @name: the variable name
 *
 * Return: the envar value (could be NULL if not set) if exists otherwise NULL
 */
char *envman_value(char *name)
{
	envar var = envman_search(name);

	return (var ? var->value : NULL);
}

/**
 * envman_set - set or remove envar
 * @name: the variable name
 * @value: the variable value
 * Description:
 * set @name and @value to set value if @name exist or to create @name with
 * @value
 *
 * set @name only to remove @name if exist
 *
 * Return: 1 on success otherwise 0 on fail
 */
int envman_set(char *name, char *value)
{
	envar head = NULL, var = NULL, last = NULL;

	var = envman_loop(1);
loop:
	if (!var)
		goto create;
	if (!_strcmp(name, var->name))
		goto update;
	last = var;
	var = envman_loop(0);
	goto loop;
create:
	var = envar_create(name, 0, value, 0);
	if (!var)
		return (0);
	head = envman_global(NULL, 0);
	if (!head)
		envman_global(var, 1);
	else if (!envar_push(&head, var))
		return (0);
	return (1);
update:
	if (value)
	{
		if (var->value)
			free(var->value);
		var->value = _strdup(value);
		return (var->value ? 1 : 0);
	}
	else
	{
		if (last)
			last->next = var->next;
		else
			envman_global(NULL, 1);
		envar_free(var, 1);
		return (1);
	}
}

/**
 * envman_arr - create or free an array of variables
 * @old_arr: array to clear from memory
 *
 * Return: the envvar cstring array otherwise NULL
 */
char **envman_arr(char **old_arr)
{
	envar var;
	size_t i = 0, size;
	char **arr = old_arr, **ptr;

	if (arr)
		goto kill;
	size = envman_size();
	var = envman_global(NULL, 0);
	arr = malloc((size + 1) * sizeof(char *));
	if (!arr)
		return (NULL);
	arr[size] = NULL;
	for (i = 0; i < size; i++)
	{
		size_t nsize = var->name ? _strlen(var->name) : 0;
		size_t vsize = var->value ? _strlen(var->value) : 0;
		size_t size = nsize + vsize + 1;

		arr[i] = malloc((size + 1) * sizeof(char));
		if (!arr[i])
			goto kill;
		arr[i][nsize] = '=';
		arr[i][size] = 0;
		if (var->name)
			_memcpy(arr[i], var->name, nsize);
		if (var->value)
			_memcpy(arr[i] + nsize + 1, var->value, vsize);
		var = var->next;
	}
	return (arr);
kill:
	ptr = arr;
	for (; *ptr; ptr++)
		free(*ptr);
	free(arr);
	return (NULL);
}

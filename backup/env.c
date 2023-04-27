#include "includes/env.h"
#include "includes/envar.h"
#include "includes/utils.h"
#include <stdlib.h>

/**
 * envman_init - initialize environment variables manager
 * @environ: the environment variables list passed to the main function
 * Description:
 * Don't forget to destroy to free memory
 *
 * if @environ is null then creates empty environ manager
 *
 * Return: 1 on success otherwise 0
 */
int envman_init(char **environ)
{
	envar head = NULL;

	for (; *environ; environ++)
	{
		envar var;
		size_t nsize = 0;

		nsize = _strspn(*environ, "=");
		for (nsize = 0; (*environ)[nsize]; nsize++)
			if ((*environ)[nsize] == '=')
				break;

		var = envar_create(*environ, nsize, (*environ) + nsize + 1, 0);
		if (!var)
			goto kill;
		if (!envar_push(&head, var))
			goto kill;
	}
	envman_global(head, 1);
	return (1);
kill:
	envar_free(head, 1);
	return (0);
}

/**
 * envman_destroy - free environ manager and its stored variables
 */
void envman_destroy(void)
{
	envar_free(envman_global(NULL, 1), 1);
}

/**
 * envman_global - sets and gets the global starting envar
 * @var: envvar to set as global starting point
 * @set: the set lock to control the stored state
 * Description:
 * set @set to true to set @var as a global value
 *
 * By default the global value is NULL
 *
 * Return: the stored global envar object
 */
envar envman_global(envar var, char set)
{
	static envar cache;
	static char hasval = -1;

	if (set)
	{
		cache = var;
		hasval = var ? 1 : -1;
	}
	return (hasval == 1 ? cache : NULL);
}

/**
 * envman_loop - loop throw variable objects
 * @rewind: if set rewinds or resets the loop to the beginning object
 * Description:
 * By default the starting point is zero so make sure to start with rewind set
 *
 * Return: the next envar object otherwise NULL
 */
envar envman_loop(char rewind)
{
	static envar cache;
	static int hasval = -1;
	envar var = NULL;

	if (rewind)
	{
		cache = envman_global(NULL, 0);
		hasval = cache ? 1 : -1;
	}
	if (hasval == -1)
		return (NULL);
	var = cache;
	cache = cache->next;
	hasval = cache ? 1 : -1;
	return (var);
}

/**
 * envman_size - calculate the size of the environment variables
 *
 * Return: number of the stored objects
 */
size_t envman_size(void)
{
	envar var = envman_global(NULL, 0);

	return (envar_size(var));
}

#include "includes/garbage.h"
#include <stdlib.h>

/**
 * garbage_global - store garbage object globaly
 * @gb: the garbage object value
 * @action: A_GET to get object and A_SET to set object mode
 * Description:
 * By default no garbage object stored
 *
 * To change the stored value use the A_SET action
 *
 * To get the stored value use the A_GET action
 *
 * Return: the stored object or NULL if no stored data, only outputs if A_GET
 */
garbage garbage_global(garbage gb, enum storage_action action)
{
	static garbage cache;
	static char has_value = -1;

	if (action == A_SET) /* check if it is the set action */
	{
		cache = gb; /* cache the new object */
		has_value = cache ? 1 : -1; /* check if cache is NULL or not */
	}
	/* only return object if action is A_GET and has cached object */
	return ((action == A_GET && has_value == 1) ? cache : NULL);
}

/**
 * garbage_last - find and return the last object in the linked list
 * @gb: the starting point for the search
 *
 * Return: the last gb object in the list otherwise NULL
 */
garbage garbage_last(garbage gb)
{
	if (!gb)
		return (NULL);
	while (gb->next)
		gb = gb->next;
	return (gb);
}

/**
 * garbage_trace - trace a new @ptr to free later
 * @scope: pointer to the scope object
 * @mode: the tracing mode
 * @ptr: the pointer to free later
 * Description:
 * Set @scope to NULL to use global scope
 *
 * Return: 1 on success otherwise 0
 */
int garbage_trace(garbage *scope, enum scope mode, void *ptr)
{
	garbage head = NULL, gb = NULL;

	if (!ptr || (mode == S_SCOPED && !scope))
		return (0);
	gb = malloc(sizeof(struct garbage_s)); /* allocate memory for the object */
	if (!gb) /* check if allocating failed */
		return (0);
	gb->ptr = ptr; /* store the pointer inside the garbage object */

	/* load the current globally stored linked list head or use scope */
	if (mode == S_GLOBAL)
		head = garbage_global(NULL, A_GET);
	else
		head = *scope ? *scope : NULL;
	if (head) /* check if a linked list head is stored */
		garbage_last(head)->next = gb; /* push to the end of the linked list */
	else if (mode == S_SCOPED)
		*scope = gb; /* if not global then set the head of the scope */
	else
		/* otherwise use the current gb object as the head of the linked list */
		garbage_global(gb, A_SET);
	return (1);
}

/**
 * garbage_free - free all traced pointers
 * @scope: free scope instead of the global scope
 * @mode: the tracing mode
 */
void garbage_free(garbage *scope, enum scope mode)
{
	garbage curr = NULL, gb = NULL;

	if (!scope && mode == S_SCOPED)
		return;
	/* load current head */
	gb = mode == S_GLOBAL ? garbage_global(NULL, A_GET) : *scope;
	while (gb)
	{
		curr = gb;
		gb = gb->next;
		free(curr->ptr); /* free the pointer */
		free(curr); /* free the garbage object */
	}
	if (mode == S_GLOBAL)
		garbage_global(NULL, A_SET); /* reset the global head */
	else if (mode == S_SCOPED)
		*scope = NULL;
}

/**
 * garbage_untrace - untrace pointer and remove from the linked list
 * @scope: use scope instead of global scope
 * @mode: the tracing mode
 * @ptr: the pointer to untrace
 */
void garbage_untrace(garbage *scope, enum scope mode, void *ptr)
{
	garbage last = NULL, gb = NULL;

	if (!ptr || (mode == S_SCOPED && !scope))
		return;
	gb = mode == S_GLOBAL ? garbage_global(NULL, A_GET) : *scope;
search:
	if (!gb)
		return;
	if (gb->ptr == ptr)
		goto remove;
	last = gb;
	gb = last->next;
	goto search;
remove:
	if (!last && mode == S_GLOBAL) /* if the gb obj is the global head itself */
		garbage_global(gb->next, A_SET); /* set the next object as the new head */
	else if (!last && mode == S_SCOPED)
		*scope = gb->next;
	else
		last->next = gb->next; /* remove from the linked list */
	free(gb); /* free the object memory */
}

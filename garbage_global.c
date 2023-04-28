#include "includes/garbage.h"

/**
 * global_trace - trace a new @ptr to free later
 * @ptr: the pointer to free later
 * Description:
 *
 * Return: 1 on success otherwise 0
 */
int global_trace(void *ptr)
{
	return (garbage_trace(NULL, S_GLOBAL, ptr));
}

/**
 * global_untrace - untrace pointer and remove from the linked list
 * @ptr: the pointer to untrace
 */
void global_untrace(void *ptr)
{
	garbage_untrace(NULL, S_GLOBAL, ptr);
}

/**
 * global_free - free all traced pointers
 */
void global_free(void)
{
	garbage_free(NULL, S_GLOBAL);
}

/**
 * global_has - check if global scope is tracing a pointer
 * @ptr: the pointer to check
 *
 * Return: 1 if found otherwise zero
 */
int global_has(void *ptr)
{
	garbage gb = garbage_global(NULL, A_GET);

	if (!ptr)
		return (0);
search:
	if (!gb)
		return (0);
	if (gb->ptr == ptr)
		return (1);
	gb = gb->next;
	goto search;
}

/**
 * global_bind - bind a scope to the global tracing scope
 * @scope: the scope object to bind
 */
void global_bind(garbage *scope)
{
	garbage head = NULL;
	int already_globaled;

	if (!scope || !*scope)
		return;
	head = garbage_global(NULL, A_GET);
	already_globaled = global_has((*scope)->ptr);
	if (!already_globaled && head)
		garbage_last(head)->next = *scope;
	else if (!already_globaled && !head)
		garbage_global(*scope, A_SET);

	*scope = NULL;
}

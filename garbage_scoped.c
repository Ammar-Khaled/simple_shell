#include "includes/garbage.h"

/**
 * scoped_trace - trace a new @ptr to free later
 * @scope: the used scope object
 * @ptr: the pointer to free later
 * Description:
 *
 * Return: 1 on success otherwise 0
 */
int scoped_trace(garbage *scope, void *ptr)
{
	return (garbage_trace(scope, S_SCOPED, ptr));
}

/**
 * scoped_untrace - untrace pointer and remove from the linked list
 * @scope: the used scope object
 * @ptr: the pointer to untrace
 */
void scoped_untrace(garbage *scope, void *ptr)
{
	garbage_untrace(scope, S_SCOPED, ptr);
}

/**
 * scoped_free - free all traced pointers
 * @scope: the used scope object
 */
void scoped_free(garbage *scope)
{
	garbage_free(scope, S_SCOPED);
}

/**
 * scoped_has - check if scope is tracing a pointer
 * @scope: pointer to the scope object
 * @ptr: the pointer to check
 *
 * Return: 1 if found otherwise 0
 */
int scoped_has(garbage *scope, void *ptr)
{
	garbage gb;

	if (!scope || !ptr)
		return (0);
	gb = *scope;
search:
	if (!gb)
		return (0);
	if (gb->ptr == ptr)
		return (1);
	gb = gb->next;
	goto search;
}

/**
 * scoped_bind - bind @src scope to the @dest scope and NULL the @src one
 * @dest_scope: the scope to copy to its end
 * @src_scope: the scope to copy from
 */
void scoped_bind(garbage *dest_scope, garbage *src_scope)
{
	int already_binded;

	if (!dest_scope || !src_scope || !*src_scope)
		return;

	already_binded = scoped_has(dest_scope, (*src_scope)->ptr);
	if (!already_binded && !*dest_scope)
		*dest_scope = *src_scope;
	else if (!already_binded && *dest_scope)
		garbage_last(*dest_scope)->next = *src_scope;
	*src_scope = NULL;	
}

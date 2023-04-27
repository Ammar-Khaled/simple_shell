#include "includes/utils.h"
#include "includes/envar.h"
#include <stdlib.h>

/**
 * envar_create - create a new envar
 * @name: the variable name
 * @nsize: name size
 * @value: the variable value
 * @vsize: value size
 * Description:
 * Note: the @name and the @value will be duplicated to new memory location
 * So it is recommended to use envar_free to free the linked list
 *
 * If @nsize or @vsize is set to zero then it will be auto calculated
 *
 * Return: envar object on success otherwise NULL
 */
envar envar_create(char *name, size_t nsize, char *value, size_t vsize)
{
	envar var;

	var = malloc(sizeof(struct envar_s));
	if (!var)
		goto end;
	if (name && !nsize)
		nsize = _strlen(name);
	var->name = !nsize ? NULL : _strndup(name, nsize);
	if (!var->name && nsize)
		goto kill;
	if (value && !vsize)
		vsize = _strlen(value);
	var->value = !vsize ? NULL : _strndup(value, vsize);
	if (!var->value && vsize)
		goto kill;
	var->next = NULL;
	return (var);
kill:
	envar_free(var, 1);
end:
	return (NULL);
}

/**
 * envar_last - finds the last envar in the linked list
 * @var: the envar object
 *
 * Return: the envar object otherwise NULL
 */
envar envar_last(envar var)
{
	if (!var)
		return (NULL);
	while (var->next)
		var = var->next;
	return (var);
}

/**
 * envar_push - push a new envar item (@var) to the linked list @head
 * @head: pointer to the head of the linked list
 * @var: the envar object to push to the list
 * Description:
 * if @head is empty or **head is NULL, then **head == @var.
 *
 * Return: @head
 */
envar *envar_push(envar *head, envar var)
{
	if (!head || !var)
		goto end;
	if (!*head)
	{
		*head = var;
		goto end;
	}
	envar_last(*head)->next = var;
end:
	return (head);
}

/**
 * envar_free - free envar linked list
 * @var: envar object to free
 * @single: set true to ignore the next variables
 */
void envar_free(envar var, char single)
{
	envar next;

loop_start:
	if (!var)
		return;
	next = var;
	if (var->name)
		free(var->name);
	if (var->value)
		free(var->value);
	var = next;
	free(var);
	if (!single)
		goto loop_start;
}

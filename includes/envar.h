#ifndef __ENVAR_H__
#define __ENVAR_H__
#include <stddef.h>

/**
 * struct envar_s - evironment variable item as a linked list item
 * @name: the variable name
 * @value: the variable value
 * @next: the next variable value
 */
typedef struct envar_s
{
	char *name;
	char *value;
	struct envar_s *next;
} *envar;

envar envar_create(char *name, size_t nsize, char *value, size_t vsize);
void envar_free(envar var, char single);
envar envar_last(envar var);
envar *envar_push(envar *head, envar var);

#endif

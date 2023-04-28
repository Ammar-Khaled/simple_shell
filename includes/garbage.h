#ifndef __GARBAGE_H__

#define __GARBAGE_H__

#ifndef NULL
#define NULL (void *)0
#endif

/**
 * enum storage_action - the global storage action
 * @A_GET: load stored object from the global storage
 * @A_SET: store an object inside the global storage
 */
enum storage_action
{ A_GET, A_SET };

/**
 * enum scope - the scope of the garbage functions
 * @S_GLOBAL: use global scope
 * @S_SCOPED: use scoped object
 */
enum scope
{ S_GLOBAL, S_SCOPED };

/**
 * struct garbage_s - a linked list struct to hold list of the traced pointers
 * @ptr: the traced pointer
 * @next: the next object in the linked list
 */
typedef struct garbage_s
{
	void *ptr;
	struct garbage_s *next;
} *garbage;

garbage garbage_global(garbage gb, enum storage_action mode);
int garbage_trace(garbage *scope, enum scope mode, void *ptr);
void garbage_untrace(garbage *scope, enum scope mode, void *ptr);
void garbage_free(garbage *scope, enum scope mode);
garbage garbage_last(garbage gb);

int global_trace(void *ptr);
void global_untrace(void *ptr);
void global_free(void);
void global_bind(garbage *scope);
int global_has(void *ptr);

int scoped_trace(garbage *scope, void *ptr);
void scoped_untrace(garbage *scope, void *ptr);
void scoped_free(garbage *scope);
void scoped_bind(garbage *dest_scope, garbage *src_scope);
int scoped_has(garbage *scope, void *ptr);

#endif

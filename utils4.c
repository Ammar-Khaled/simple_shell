#include "includes/env.h"
#include <stdlib.h>

/**
 * min - find min of two nums
 * @a: first num
 * @b: second num
 * Return: min of a and b
*/
int min(int a, int b)
{
	return ((a < b) ? (a) : (b));
}

/**
 * _memset - set memory piece of memory to byte
 * @ptr: the starting pointer
 * @byte: the byte value to set
 * @nbytes: the number of bytes to be written starting for @ptr
 *
 * Return: @ptr otherwise NULL on fail
 */
void *_memset(void *ptr, char byte, unsigned int nbytes)
{
	if (!ptr)
		return (NULL);
	for (; nbytes; nbytes--)
		((char *)ptr)[nbytes - 1] = byte;
	return (ptr);
}


/**
 * _memcpy - copy @nbytes bytes from @src to @dest
 * @dest: pointer to start writting to
 * @src: pointer to start reading from
 * @nbytes: number of bytes to be copied
 *
 * Return: @dest on success otherwise NULL on fail
 */
void *_memcpy(void *dest, void *src, unsigned int nbytes)
{
	void *org = dest;
	size_t i;

	for (i = 0; i < nbytes; i++)
		((char *)dest)[i] = ((char *)src)[i];
	return (org);
}

/**
 * _realloc - reallocates a memory block using malloc and free
 * @ptr:	a pointer to the memory previously allocated with a call to malloc:
 * malloc(old_size).
 * @old_size: is the size, in bytes, of the allocated space for ptr.
 * @new_size: is the new size, in bytes of the new memory block.
 * Return: a pointer to the newly reallocated memory block
 */
void *_realloc(void *ptr, unsigned int old_size, unsigned int new_size)
{
	void *newptr;

	if (ptr == NULL)
		return (malloc(new_size));
	if (new_size == 0)
	{
		free(ptr);
		return (NULL);
	}
	if (old_size == new_size)
		return (ptr);
	newptr = malloc(new_size);
	if (!newptr)
		goto kill;
	new_size = min(old_size, new_size);
	_memcpy(newptr, ptr, min(old_size, new_size));
	free(ptr);
	return (newptr);
kill:
	free(ptr);
	return (NULL);
}

/**
 * super_nova - free all global memory the exit with status
 * @status: the exit status
 */
void super_nova(int status)
{
	envman_destroy();
	exit(status);
}

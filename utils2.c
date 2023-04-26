#include "includes/utils.h"
#include <stdlib.h>

/**
 * _strdup - duplicate cstring in new allocated memory
 * @str: pointer to the cstring
 *
 * Return: pointer of the new allocated memory otherwise NULL
 */
char *_strdup(char *str)
{
	char *ptr;
	size_t size;

	if (!str)
		return (NULL);
	size = _strlen(str);
	ptr = malloc(size + 1);
	if (!ptr)
		return (NULL);
	ptr = _strcpy(ptr, str);
	return (ptr);
}

/**
 * _strcpy - copy cstring from @src to @dest
 * @dest: pointer to the dest cstring
 * @src: pointer to the src cstring
 *
 * Return: @dest on success otherwise NULL
 */
char *_strcpy(char *dest, char *src)
{
	char *org;

	if (!dest || !src)
		return (NULL);
	for (org = dest; *src; src++, dest++)
		*dest = *src;
	*dest = *src;
	return (org);
}

/**
 * _strcmp - compare @nbytes of 2 strings
 * @s1: first string
 * @s2: second string
 * @nbytes: number of bytes to be compared
 * Description:
 * if zero then both are equal otherwise
 * if positive @s1 is greater than @s2 finally if negative @s2 > @s1
 *
 * Return: zero if both are equal or +ve if @s1 > @s2; -ve if @s2 > @s1
 */
int _strncmp(char *s1, char *s2, size_t nbytes)
{
	int d = 0;

	while (nbytes)
	{
		d = *s1 - *s2;
		if (d || !*s1 || !*s2)
			return d;
		s1++, s2++, nbytes--;
	}
	return (d);
}

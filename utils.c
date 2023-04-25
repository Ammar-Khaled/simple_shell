#include "includes/utils.h"

/**
 * _strchr - search for character in string
 * @str: string to search in
 * @c: character to search for
 *
 * Return: pointer to first accurance of @c in @str otherwise NULL
 */
char *_strchr(char *str, char c)
{
	if (!str || !c)
		return (NULL);
	for (; *str; str++)
		if (*str == c)
			return (str);
	return (NULL);
}

/**
 * _strspn - calc the len of the continues accurance of @delim chars in @str
 * @str: string to search in
 * @delim: characters to use in search
 *
 * Return: the calculated length
 */
size_t _strspn(char *str, char *delim)
{
	size_t size = 0;

	if (!str || !delim)
		return (0);
	for (; *str; str++, size++)
		if (!_strchr(delim, *str))
			break;
	return (size);
}

/**
 * _strtok - strtok implementation
 * @str: string to split
 * @delim: characters to be used as a delimeter
 *
 * Return: pointer to the start of the token in @str
 */
char *_strtok(char *str, char *delim)
{
	static char *ptr;
	char *token;

	/* set string to tokenize if any */
	if (str)
		ptr = str;
	/* if no any data to tokenize then end */
	if (!ptr || !*ptr)
		return (NULL);

	/* skip beginning*/
	ptr += _strspn(ptr, delim);
	/* if no more chars then end */
	if (ptr && !*ptr)
		return (NULL);
	/* set the beginning of the token */
	token = ptr;
	/* go to the next delim */
	for (; *ptr; ptr++)
		if (_strchr(delim, *ptr))
			break;
	/* if not the end of string then end the token */
	if (*ptr)
		*ptr++ = 0;
	return (token);
}

#include <string.h>

/**
 * _strtok - strtok implementation
 * @str: string to split
 * @delim: characters to be used as a delimeter
 *
 * Return: pointer to the start of the token in @str
 */
char *_strtok(char *str, const char *delim)
{
	char *start, *end;
	static char *cache;

	if (str)
		cache = str;
	if (!cache)
		return (NULL);

	start = cache;
	end = strpbrk(cache, delim);
	if (end)
	{
		*end = 0;
		cache = end + 1;
	}
	else
	{
		cache = NULL;
	}
	return (start);
}

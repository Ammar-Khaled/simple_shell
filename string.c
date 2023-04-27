#include "includes/string.h"
#include <ctype.h>
#include <stddef.h>
#include <string.h>

/**
 * _strtrim - trim string from left and right
 * @str: the string to trim
 * Description:
 * just returns the end of the whitespaces and set NULL char at the end of str
 *
 * Return: pointer to the begin of the trimed string
 */
char *_strtrim(char *str)
{
	char *end;

	if (!str || !*str)
		return (str);

	while (isspace(*str))
		str++;

	if (!*str)
		return (str);

	end = str + strlen(str) - 1;
	while (isspace(*end))
		end--;
	if (*end)
		end[1] = 0;
	return (str);
}

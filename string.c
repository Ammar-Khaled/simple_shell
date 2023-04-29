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
	ptr += strspn(ptr, delim);
	/* if no more chars then end */
	if (ptr && !*ptr)
		return (NULL);
	/* set the beginning of the token */
	token = ptr;
	/* go to the next delim */
	for (; *ptr; ptr++)
		if (strchr(delim, *ptr))
			break;
	/* if not the end of string then end the token */
	if (*ptr)
		*ptr++ = 0;
	return (token);
}

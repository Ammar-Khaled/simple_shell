#include <stdlib.h>

/**
 * _strtrim - removes leading and trialing spaces from string
 * @str: string
 *
 * Return: NULL if the string is all spaces, otherwise returns
 * the string after trimming
 */
char *_strtrim2(char *str)
{
	int len;
	char *end;

	/* Trim leading whitespace */
	while (*str == ' ')
		str++;

	/* check if the entire string is just spaces */
	if (*str == '\0')
		return (NULL);

	/* calculate length of the string */
	for (len = 0; str[len]; len++)
		;

	end = str + len - 1;

	/* Trim trailing white spaces */
	while (end > str && *end == ' ')
		end--;

	/* NULL terminating the string */
	*(end + 1) = '\0';

	return (str);
}

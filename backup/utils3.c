#include "includes/utils.h"
#include <stdlib.h>

/**
 * print - write string to file descriptor
 * @fd: file descriptor
 * @str: cstring to be written
 * @newlines: number of newlines to append at the end
 *
 * Return: number of written bytes otherwise -1 on error
 */
ssize_t print(int fd, char *str, unsigned int newlines)
{
	char *newstr = NULL;
	ssize_t nread = 0;
	size_t size = str ? _strlen(str) : 0, newsize = size + newlines;

	if (newlines)
	{
		newstr = malloc(size + newlines + 1);
		if (!newstr)
			return (-1);
		if (str)
			newstr = _strcpy(newstr, str);
		newstr[newsize] = 0;
		_memset(newstr + size, 10, newlines);
	}

	if (newstr || str)
		nread = write(fd, newlines ? newstr : str, newsize);
	return (nread);
}

/**
 * printo - print cstring to stdout
 * @str: string to print
 * @newlines: number of newlines to append at the end
 *
 * Return: check print function
 */
ssize_t printo(char *str, unsigned int newlines)
{
	return (print(STDOUT_FILENO, str, newlines));
}

/**
 * printu - write unsigned number as csting to file
 * @fd: the file descriptor
 * @num: the number to convert to ascii and write
 * @newlines: number of newlines to append at the end
 *
 * Return: number of written bytes otherwise -1
 */
ssize_t printu(int fd, unsigned long num, unsigned int newlines)
{
	char str[20];

	if (!number_converter(str, num, 10))
		return (-1);
	return (print(fd, str, newlines));
}

/**
 * printuo - write unsigned number as csting to stdout
 * @num: the number to convert to ascii and write
 * @newlines: number of newlines to append at the end
 *
 * Return: number of written bytes otherwise -1
 */
ssize_t printuo(unsigned long num, unsigned int newlines)
{
	return (printu(STDOUT_FILENO, num, newlines));
}

/**
 * number_converter - convert decimal number into string in @base form
 * @str: string buffer to write the output at
 * @value: the decimal number to be converted
 * @base: the base to convert the decimal number @value to
 * Return: 1 if success otherwise 0
 */
int number_converter(char *str, unsigned long value, int base)
{
	int i = 0;
	char *dict = "0123456789abcdef";

	if (base < 2 || base > 16)
		return (0);

	while (value || !i)
	{
		str[i++] = dict[value % base];
		value /= base;
	}
	str[i] = 0;

	_strrev(str, i);
	return (1);
}


#include "includes/utils.h"

/**
 * print - write string to file descriptor
 * @fd: file descriptor
 * @str: cstring to be written
 *
 * Return: number of written bytes otherwise -1 on error
 */
ssize_t print(int fd, char *str)
{
	size_t size = _strlen(str);

	return (write(fd, str, size));
}

/**
 * printo - print cstring to stdout
 * @str: string to print
 *
 * Return: check print function
 */
ssize_t printo(char *str)
{
	return (print(STDOUT_FILENO, str));
}

/**
 * printu - write unsigned number as csting to file
 * @fd: the file descriptor
 * @num: the number to convert to ascii and write
 *
 * Return: number of written bytes otherwise -1
 */
ssize_t printu(int fd, unsigned long num)
{
	char str[20];

	if (!number_converter(str, num, 10))
		return (-1);
	return (print(fd, str));
}

/**
 * printuo - write unsigned number as csting to stdout
 * @num: the number to convert to ascii and write
 *
 * Return: number of written bytes otherwise -1
 */
ssize_t printuo(unsigned long num)
{
	return (printu(STDOUT_FILENO, num));
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


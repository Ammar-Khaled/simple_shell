#include "includes/main.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

/**
 * show_prompt - show prompt only if the stream is a tty
 * @stream: the input stream
 */
void show_prompt(FILE *stream)
{
	int fd = fileno(stream);

	if (isatty(fd))
		write(fd, "$ ", 2);
}

/**
 * read_command - read user input
 * @stream: the stream to read the input from
 * @lineptr: pointer to the char *lineptr variable
 * @size: pointer to the input size variable
 *
 * Return: +ve for readed chars, -1 for getline error, -2 for reading end
 */
int read_command(FILE *stream, char **lineptr, size_t *size)
{
	size_t linesize = 0;
	ssize_t nread = 0;

	nread = getline(lineptr, &linesize, stream);
	*size = nread >= 0 ? nread : 0;
	if (nread > 0)
		(*lineptr)[*size - 1] = 0;
	else if (errno != EINVAL && errno != ENOMEM)
		nread = READ_END;
	return (nread);
}

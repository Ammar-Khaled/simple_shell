#include "includes/main.h"
#include "includes/memory.h"
#include "includes/string.h"
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
 * @filename: the executable filename
 *
 * Return: +ve for readed chars, -1 for getline error, -2 for reading end
 */
int read_command(FILE *stream, char **lineptr, size_t *size, char *filename)
{
	size_t linesize = 0;
	ssize_t nread = 0;

	nread = getline(lineptr, &linesize, stream);
	*size = nread >= 0 ? nread : 0;
	if (nread > 0)
		(*lineptr)[*size - 1] = 0;
	else if (errno == EINVAL || errno == ENOMEM)
	{
		perror(filename);
		return (-1);
	}
	else
		return (READ_END);
	return (nread);
}

/**
 * split_command - splits the input line into tokens to execute
 * @lineptr: the input line
 *
 * Return: an allocated null terminated list of arguments or tokens
 */
char **split_command(char *lineptr)
{
	char *token, **args;
	size_t count = 0, size = 5;

	if (!lineptr)
		return (NULL);

	args = malloc(size * sizeof(char *));
	if (!args)
		return (NULL);

	lineptr = _strtrim(lineptr);
	token = strtok(lineptr, " \n\r\t\v");
	while (token)
	{
		args[count++] = token;
		if (count == size)
		{
			args = _realloc(args,
				size * sizeof(char *), (size + 5) * sizeof(char *));
			size += 5;
			if (!args)
				return (NULL);
		}
		token = strtok(NULL, " \n\r\t\v");
	}
	args[count] = NULL;
	return (args);
}

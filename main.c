#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "core.h"

/**
 * main - simple shell
 * @argc: argument counter
 * @argv: argument vector
 * @environ: environment
 *
 * Return: 0 for success
 */
int main(int argc, char **argv, char **environ)
{
	char *name;
	char *lineptr;
	size_t linesize;
	ssize_t nread;
	char **args;

	name = argv[0];

	/* non-interactive mode: excute one command */
	if (argc > 1)
	{
		int status = evaluate(name, argv, environ);

		exit(status);
	}

	/* inter-active mode */
begin:
	printf("($) ");
	nread = getline(&lineptr, &linesize, stdin); /* read line */
	if (feof(stdin))
		goto clean;
	if (nread == -1)
	{
		perror(name);
		goto begin;
	}
	lineptr[strlen(lineptr) - 1] = 0; /* remove last byte (delim) */
	args = splitline(lineptr);
	evaluate(name, args, environ);
	goto begin;
clean:
	free(lineptr);
	return (0);
}

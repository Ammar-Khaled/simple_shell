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
	char *name = argv[0];
	size_t linesize;
	char *lineptr;
	char **args;

	if (argc > 1)
		exit(evaluate(name, argv, environ));
begin:
	readline(name, "> ", &lineptr, &linesize);
	if (linesize == 0)
		goto begin;
	args = splitline(lineptr);
	evaluate(name, args, environ);
	free(lineptr);
	goto begin;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "core.h"

/**
 * main - simple shell
 * @argc: argument counter
 * @argv: argument vector
 *
 * Return: 0 for success
 */
int main(int argc, char **argv)
{
	char *name = argv[0];
	size_t linesize;
	char *lineptr;
	char **args;

	if (argc > 1)
		exit(evaluate(name, argv));
begin:
	readline(name, "> ", &lineptr, &linesize);
	if (linesize == 0)
		goto begin;
	args = splitline(lineptr);
	evaluate(name, args);
	free(lineptr);
	goto begin;
}

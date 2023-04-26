#include <stdio.h>
#include <stdlib.h>
#include "includes/core.h"

/**
 * main - simple shell
 * @argc: argument counter
 * @argv: argument vector
 *
 * Return: 0 for success
 */
int main(int argc, char **argv)
{
	size_t linesize;
	char *lineptr, **args;

	/* set shell env vars */
	putenv("SHELL=hsh");
	putenv("PROMPT=>");
	setenv("SHELL_EXEC", argv[0], 1);

	if (argc > 1)
		exit(evaluate(argv));
begin:
	readline(&lineptr, &linesize);
	if (linesize == 0)
		goto begin;
	args = splitline(lineptr);
	evaluate(args);
	free(lineptr);
	goto begin;
}

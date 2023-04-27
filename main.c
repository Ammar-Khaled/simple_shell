#include "includes/core.h"
#include "includes/env.h"
#include "includes/utils.h"
#include <stdlib.h>

/**
 * main - simple shell
 * @argc: argument counter
 * @argv: argument vector
 * @environ: list of environment variables
 *
 * Return: 0 for success
 */
int main(int argc, char **argv, char **environ)
{
	size_t linesize;
	char *lineptr, **args;

	/* set shell env vars */
	envman_init(environ);
	envman_set("SHELL", "hsh");
	envman_set("PROMPT", "$");
	envman_set("SHELL_EXEC", argv[0]);

	if (argc > 1)
		super_nova(evaluate(argv));
begin:
	readline(&lineptr, &linesize);
	if (linesize == 0)
		goto begin;
	args = splitline(lineptr);
	evaluate(args);
	free(lineptr);
	goto begin;
}

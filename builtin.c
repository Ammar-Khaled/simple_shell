#include "includes/main.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <limits.h>
#include <unistd.h>

/**
 * builtin_exit - exit the shell
 * @ctx: the command line context object
 * Description:
 * exits the shell with optional status code
 *
 * Return: status code
 */
int builtin_exit(context *ctx)
{
	int i, state = 0;
	char *num;

	if (ctx->argc > 1)
	{
		num = ctx->args[1];
		for (i = 0; num[i]; i++)
			if (!isdigit(num[i]))
			{
				print_error(ctx, "Illegal number", num);
				return (2);
			}
		state = atoi(num);
	}

	ctx->signal = S_EXIT;
	return (state);
}

/**
 * builtin_env - prints all environment variables
 * @ctx: the command line context object
 * Description:
 *
 * Return: status code
 */
int builtin_env(context *ctx __attribute__((unused)))
{
	char **env = environ;

	for (; *env; env++)
		printf("%s\n", *env);
	return (0);
}

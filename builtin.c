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

/**
 * builtin_setenv - set or modify exist environment variable
 * @ctx: the context object
 *
 * Return: exit status
 */
int builtin_setenv(context *ctx)
{
	if (ctx->argc != 3)
	{
		print_error(ctx, "Invalid Arguments", NULL);
		return (EXIT_FAILURE);
	}
	setenv(ctx->args[1], ctx->args[2], 1);
	return (EXIT_SUCCESS);
}

/**
 * builtin_unsetenv - unsets an existing environment variables
 * @ctx: the context object
 *
 * Return: exit status
 */
int builtin_unsetenv(context *ctx)
{
	if (ctx->argc != 2)
	{
		print_error(ctx, "Invalid Arguments", NULL);
		return (EXIT_FAILURE);
	}

	if (!getenv(ctx->args[1]))
	{
		print_error(ctx, "variable not set", ctx->args[1]);
		return (2);
	}
	unsetenv(ctx->args[1]);
	return (EXIT_SUCCESS);
}

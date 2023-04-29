#include "includes/main.h"
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
	ctx->signal = S_EXIT;
	return (0);
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

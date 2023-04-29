#include "includes/main.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <limits.h>
#include <unistd.h>

/**
 * builtin_exit - exit the program
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

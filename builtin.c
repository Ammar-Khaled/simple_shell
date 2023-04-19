#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "builtin.h"

/**
 * builtin_exit - exit the program
 * @argc: number of arguments
 * @argv: list of arguments
 * Description:
 * exits the shell with optional status code
 *
 * Return: status code
 */
int builtin_exit(int argc, char **argv)
{
	int status = 0;

	if (argc > 2)
	{
		dprintf(STDERR_FILENO, "Usage: %s [status]\n", argv[0]);
		return (1);
	}

	if (argc == 2)
		status = atoi(argv[1]);
	exit(status);
}

/**
 * builtin_env - prints environment variables
 * @argc: number of arguments
 * @argv: list of arguments
 * Description:
 * prints the environment variables
 *
 * Return: status code
 */
int builtin_env(int argc __attribute__((unused)),
								char **argv __attribute__((unused)))
{
	char **vars = environ;

	for (; *vars; vars++)
		printf("%s\n", *vars);
	return (0);
}

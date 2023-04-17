#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/**
 * builtin_exit - exit the program
 * @argc: number of arguments
 * @argv: list of arguments
 * @environ: list of environment variables
 * Description:
 * exits the shell with optional status code
 *
 * Return: status code
 */
int builtin_exit(int argc, char **argv, char **environ __attribute__((unused)))
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
 * @environ: list of environment variables
 * Description:
 * prints the environment variables
 *
 * Return: status code
 */
int builtin_env(int argc __attribute__((unused)),
								char **argv __attribute__((unused)),
								char **environ)
{
	for (; *environ; environ++)
		printf("%s\n", *environ);
	return (0);
}

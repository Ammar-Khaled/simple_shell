#include <linux/limits.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "includes/builtin.h"

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
int builtin_env(int argc, char **argv)
{
	char **vars = environ;

	if (argc == 1)
		for (; *vars; vars++)
			printf("%s\n", *vars);
	else if (argc == 2)
		printf("%s=%s\n", argv[1], getenv(argv[1]));
	else
		fprintf(stderr, "Usage: env [name]\n");
	return (0);
}

/**
 * builtin_setenv - set or modify exist environment variable
 * @argc: number of arguments
 * @argv: arguments list
 *
 * Return: exit status
 */
int builtin_setenv(int argc, char **argv)
{
	if (argc != 3)
	{
		fprintf(stderr, "Usage: setenv VARIABLE VALUE\n");
		return (1);
	}

	setenv(argv[1], argv[2], 1);
	return (0);
}

/**
 * builtin_unsetenv - unsets an existing environment variables
 * @argc: number of arguments
 * @argv: arguments list
 * Description:
 * The variable to unset must be exist
 *
 * Return: exit status
 */
int builtin_unsetenv(int argc, char **argv)
{
	if (argc != 2)
	{
		fprintf(stderr, "Usage: unsetenv VARIABLE\n");
		return (1);
	}

	if (!getenv(argv[1]))
	{
		fprintf(stderr,
			"%s: 9: unsetenv: variable name '%s' is not exist\n",
			getenv("SHELL_EXEC"),
			argv[1]);
		return (1);
	}

	setenv(argv[1], argv[2], 1);
	return (0);
}

/**
 * builtin_cd - change current working directory
 * @argc: number of arguments
 * @argv: list of arguments
 * Description:
 * can cd to HOME to last working directory
 *
 * Return: status code
 */
int builtin_cd(int argc, char **argv)
{
	struct stat statbuf;
	char cwd[PATH_MAX], *path;

	if (argc > 2)
	{
		fprintf(stderr, "Usage: cd [path|-|~]\n");
		return (1);
	}
	if (argc == 1)
		path = getenv("HOME"); /* if no args set path to HOME */
	else if (argc == 2)
		path = argv[1]; /* get first arg as a path if exist */

	if (!strncmp(path, "-", 1))
		path = getenv("OLDPWD"); /* cd to last working directory */
	else if (!strncmp(path, "~", 1))
		path = getenv("HOME"); /* cd to HOME directory */

	/* if no path or not exist then exit with failure */
	if (!path)
		return (1);
	else if (stat(path, &statbuf) == -1)
	{
		perror("cd");
		return (1);
	}
	else if (!S_ISDIR(statbuf.st_mode))
	{
		fprintf(stderr, "cd: The directory '%s' is not exist\n", path);
		return (1);
	}

	setenv("OLDPWD", getenv("PWD"), 1); /* update OLDPWD env var */
	chdir(path); /* change current working directory */
	if (getcwd(cwd, PATH_MAX))
		setenv("PWD", cwd, 1); /* update PWD env var */
	return (0);
}

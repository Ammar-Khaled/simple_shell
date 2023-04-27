#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <limits.h>
#include <unistd.h>
#include "includes/builtin.h"
#include "includes/env.h"
#include "includes/utils.h"

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
		print(STDERR_FILENO, "Usage: ", 0);
		print(STDERR_FILENO, argv[0], 0);
		print(STDERR_FILENO, " [status]", 1);
		return (1);
	}

	if (argc == 2)
		status = atoi(argv[1]);
	super_nova(status);
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
	envar var;

	if (argc == 1)
	{
		var = envman_loop(1);
		while (var)
		{
			printo(var->name, 0);
			printo("=", 0);
			printo(var->value, 1);
			var = envman_loop(0);
		}
	}
	else if (argc == 2)
	{
		printo(argv[1], 0);
		printo("=", 0);
		printo(envman_value(argv[1]), 1);
	}
	else
		print(STDERR_FILENO, "Usage: env [name]", 1);
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
		print(STDERR_FILENO, "Usage: setenv VARIABLE VALUE", 1);
		return (1);
	}

	envman_set(argv[1], argv[2]);
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
		print(STDERR_FILENO, "Usage: unsetenv VARIABLE", 1);
		return (1);
	}

	if (!envman_value(argv[1]))
	{
		print(STDERR_FILENO, envman_value("SHELL_EXEC"), 0);
		print(STDERR_FILENO, ": 9: unsetenv: variable name '", 0);
		print(STDERR_FILENO, argv[1], 0);
		print(STDERR_FILENO, "' is not exist", 1);
		return (1);
	}

	envman_set(argv[1], argv[2]);
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
		print(STDERR_FILENO, "Usage: cd [path|-|~]", 1);
		return (1);
	}
	if (argc == 1)
		path = envman_value("HOME"); /* if no args set path to HOME */
	else if (argc == 2)
		path = argv[1]; /* get first arg as a path if exist */

	if (!_strncmp(path, "-", 1))
		path = envman_value("OLDPWD"); /* cd to last working directory */
	else if (!_strncmp(path, "~", 1))
		path = envman_value("HOME"); /* cd to HOME directory */

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
		print(STDERR_FILENO, "cd: The directory '", 0);
		print(STDERR_FILENO, path, 0);
		print(STDERR_FILENO, "' is not exist", 1);
		return (1);
	}

	envman_set("OLDPWD", envman_value("PWD")); /* update OLDPWD env var */
	chdir(path); /* change current working directory */
	if (getcwd(cwd, PATH_MAX))
		envman_set("PWD", cwd); /* update PWD env var */
	return (0);
}

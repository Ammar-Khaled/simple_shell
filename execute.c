#include "includes/main.h"
#include <limits.h>
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <dirent.h>
#include <unistd.h>

/**
 * cmd_get_path - search and get the command path
 * @cmd: the command to look for
 *
 * Return: allocated command fullpath otherwise NULL
 */
char *cmd_get_path(char *cmd)
{
	char *token, *resolved, fullpath[PATH_MAX], *path;

	if (!cmd)
		return (NULL);
	resolved = realpath(cmd, NULL);
	if (resolved)
		return (resolved);
	path = getenv("PATH");
	if (!path)
		return (NULL);
	path = strdup(path);
	if (!path)
		return (NULL);

	token = strtok(path, ":");
	while (token)
	{
		snprintf(fullpath, PATH_MAX, "%s/%s", token, cmd);
		resolved = realpath(fullpath, NULL);
		if (resolved)
			goto found;
		token = strtok(NULL, ":");
	}
	free(path);
	return (NULL);
found:
	free(path);
	return (resolved);
}

/**
 * execute - execute command
 * @filename: the shell file name
 * @args: the command and its arguments to be executed
 *
 * Return: execute function state
 */
int execute(char *filename, char **args)
{
	char *cmd_name, *cmd_path;
	pid_t pid = -5;
	int state = -1;

	if (!filename || !args || !*args || !environ)
		return (0);

	cmd_name = args[0];
	cmd_path = cmd_get_path(cmd_name);

	if (cmd_path)
		pid = fork();
	else
		fprintf(stderr, "%s: %d: %s: not found\n", filename, 1, cmd_name);
	if (!pid)
	{
		execve(cmd_path, args, environ);
		perror(filename);
		state = -1;
	}
	else if (pid == -1)
	{
		perror(filename);
		state = -1;
	}
	else if (pid > 0)
	{
		do {
			waitpid(pid, &(state), WUNTRACED);
		} while (!WIFEXITED(state) && !WIFSIGNALED(state));
		state = WEXITSTATUS(state);
	}
	if (cmd_path)
		free(cmd_path);
	return (state);
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include "process.h"
#include "core.h"

/**
 * getpaths - reads the environment variables and returns the PATH var
 * @environ: the environment variables list
 *
 * Return: list of paths in the PATH env var otherwise NULL
 */
char **getpaths(char *const *environ)
{
	size_t i = 0, size = 5;
	char **paths = NULL, *token;

	/* look for PATH env var */
	for (; *environ; environ++)
		if (!strncmp("PATH=", *environ, 5))
			goto start;
	return (NULL); /* return NULL if not found */
start:
	/* allocate memory for paths list */
	paths = calloc(size, sizeof(void *));
	if (!paths)
		return (NULL);

	token = _strtok((*environ) + 5, PATH_DELIM);
	while (token)
	{
		paths[i++] = token;
		if (i >= size)
		{
			size += 5;
			paths = realloc(paths, size * sizeof(void *));
			if (!paths)
				return (NULL);
		}
		token = _strtok(NULL, PATH_DELIM);
	}
	paths[i] = NULL;
	return (paths);
}

/**
 * cmdpath - search PATH env var for the fullpath of the command
 * @cmd: the command name to look for
 * @environ: a list of the environment variables that contains PATH var
 *
 * Return: the fullpath of the command executable if exist otherwise NULL
 */
char *cmdpath(char *cmd, char *const *environ)
{
	char **paths = getpaths(environ), fullpath[FULLPATH_MAX];
	DIR *dirp;
	struct dirent *entry;

	for (; paths && *paths; paths++)
	{
		dirp = opendir(*paths);
		if (!dirp)
			continue;
begin:
		entry = readdir(dirp);
		if (!entry)
			goto end;

		snprintf(fullpath, FULLPATH_MAX, "%s/%s", *paths, entry->d_name);
		if (access(fullpath, F_OK | X_OK) == -1)
			goto begin;
		if (!strcmp(entry->d_name, cmd))
		{
			closedir(dirp);
			return (strdup(fullpath));
		}

		goto begin;
end:
		closedir(dirp);
	}
	return (NULL);
}

/**
 * execute - executes the exectable program files
 * @name: name of our program (required for error handling)
 * @cmd: array of strings contains the command and its arguments
 * @environ: environment
 *
 * Return: exit status of child otherwise -1 on fail and prints error message
 */
int execute(const char *name, char **cmd, char *const *environ)
{
	char *path;
	pid_t pid;
	int wstatus;

	if (!name || !cmd || !environ)
		return (-1);
	/* use command if its path is valid otherwise search for its executable */
	if (access(cmd[0], X_OK) != -1)
		path = cmd[0];
	else
		path = cmdpath(cmd[0], environ);
	if (!path)
	{
		dprintf(STDERR_FILENO, "%s: 1: %s: not found!\n", name, cmd[0]);
		return (-1);
	}

	pid = fork();
	if (!pid)
	{
		execve(path, cmd, environ);
		perror(name);
		exit(EXIT_FAILURE);
	}
	else if (pid == -1)
	{
		perror(name);
		return (-1);
	}

	do {
		waitpid(pid, &wstatus, WUNTRACED);
	} while (!WIFEXITED(wstatus) && !WIFSIGNALED(wstatus));
	return (WEXITSTATUS(wstatus)); /* return child exit status */
}

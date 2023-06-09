#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include "includes/env.h"
#include "includes/process.h"
#include "includes/utils.h"

/**
 * cmdpath - search PATH env var for valid executable
 * @cmd: the command name to look for
 * Description:
 * You need to free the returned fullpath if it is not NULL
 *
 * Return: the fullpath of the executable if exist otherwise NULL
 */
char *cmdpath(char *cmd)
{
	char *pathvar, *path, fullpath[PATH_MAX];
	DIR *dirp;
	struct dirent *entry;

	pathvar = envman_value("PATH"); /* get the path env var */
	pathvar = _strdup(pathvar); /* duplicate to prevent modifying PATH */
	if (!pathvar) /* if no PATH env var goto end */
		goto end;
	path = _strtok(pathvar, PATH_DELIM); /* get the first path token */
loop:
	if (!path) /* if no first path or no paths any more goto end */
		goto end;
	dirp = opendir(path); /* try to open a path stream to get its entries */
	if (!dirp) /* on fail goto next path token */
		goto next_path_token;
next_entry: /* get the next dir entry and check if it is a valid entry */
	entry = readdir(dirp); /* get entry from the dir stream */
	if (!entry) /* if not more dir entries goto the next path token */
		goto next_path_token;
	/* define the full file path as "{path_token}/{entry_name}" */
	snprintf(fullpath, PATH_MAX, "%s/%s", path, entry->d_name);
	/* check if the path is an exist regular file and executable */
	if (access(fullpath, F_OK | X_OK) == -1)
		goto next_entry; /* if invalid path then goto next entry */
	/* check if the file has the same command name */
	if (!_strcmp(entry->d_name, cmd))
	{
		closedir(dirp); /* close currently opened dir stream */
		return (_strdup(fullpath)); /* return a duplicated fullpath */
	}
	goto next_entry; /* if invalid entry then goto next one */
next_path_token:
	if (dirp) /* close current dir stream if opened */
		closedir(dirp);
	path = _strtok(NULL, PATH_DELIM);
	goto loop;
end:
	return (NULL);
}

/**
 * execute - executes the exectable program files
 * @cmd: array of strings contains the command and its arguments
 *
 * Return: exit status of child otherwise -1 on fail and prints error message
 */
int execute(char **cmd)
{
	char *path, **environ;
	pid_t pid;
	int wstatus;

	if (!cmd)
		return (-1);
	/* use cmd if it is a valid path otherwise search for its executable path */
	path = access(cmd[0], F_OK | X_OK) != -1 ? _strdup(cmd[0]) : cmdpath(cmd[0]);
	if (!path)
	{
		print(STDERR_FILENO, envman_value("SHELL_EXEC"), 0);
		print(STDERR_FILENO, ": 1: ", 0);
		print(STDERR_FILENO, cmd[0], 0);
		print(STDERR_FILENO, ": not found!", 1);
		return (-1);
	}

	pid = fork();
	if (!pid)
	{
		environ = envman_arr(NULL); /* create array */
		execve(path, cmd, environ);
		envman_arr(environ); /* free array */
		perror(envman_value("SHELL_EXEC"));
		super_nova(EXIT_FAILURE);
	}
	else if (pid == -1)
	{
		perror(envman_value("SHELL_EXEC"));
		return (-1);
	}

	free(path);
	do {
		waitpid(pid, &wstatus, WUNTRACED);
	} while (!WIFEXITED(wstatus) && !WIFSIGNALED(wstatus));
	return (WEXITSTATUS(wstatus)); /* return child exit status */
}

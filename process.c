#include <linux/limits.h>
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

	pathvar = getenv("PATH"); /* get the path env var */
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
	if (!strcmp(entry->d_name, cmd))
	{
		closedir(dirp); /* close currently opened dir stream */
		return (strdup(fullpath)); /* return a duplicated fullpath */
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
 * @name: name of our program (required for error handling)
 * @cmd: array of strings contains the command and its arguments
 *
 * Return: exit status of child otherwise -1 on fail and prints error message
 */
int execute(char **cmd)
{
	char *path;
	pid_t pid;
	int wstatus;

	if (!cmd || !environ)
		return (-1);
	/* use cmd if it is a valid path otherwise search for its executable path */
	path = access(cmd[0], F_OK | X_OK) != -1 ? strdup(cmd[0]) : cmdpath(cmd[0]);
	if (!path)
	{
		fprintf(stderr, "%s: 1: %s: not found!\n", getenv("SHELL_EXEC"), cmd[0]);
		return (-1);
	}

	pid = fork();
	if (!pid)
	{
		execve(path, cmd, environ);
		perror(getenv("SHELL_EXEC"));
		exit(EXIT_FAILURE);
	}
	else if (pid == -1)
	{
		perror(getenv("SHELL_EXEC"));
		return (-1);
	}

	free(path);
	do {
		waitpid(pid, &wstatus, WUNTRACED);
	} while (!WIFEXITED(wstatus) && !WIFSIGNALED(wstatus));
	return (WEXITSTATUS(wstatus)); /* return child exit status */
}

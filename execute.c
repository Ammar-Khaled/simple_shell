#include "includes/main.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

/**
 * execute - execute command
 * @filename: the shell file name
 * @args: the command and its arguments to be executed
 *
 * Return: execute function state
 */
int execute(char *filename, char **args)
{
	char cmd_path[PATH_MAX], *cmd_name;
	pid_t pid = 0;
	int state = 0;

	if (!filename || !args || !environ)
		return (-1);

	pid = fork();
	if (!pid)
	{
		if (args[0])
		{
			cmd_name = args[0];
			args[0] = realpath(args[0], cmd_path);
		}
		if (!args[0])
		{
			fprintf(stderr, "%s: %d: %s: not found\n", filename, 1, cmd_name);
			return (-1);
		}
		execve(args[0], args, environ);
		perror(filename);
		return (-1);
	}
	else if (pid == -1)
	{
		perror(filename);
		return (-1);
	}
	else if (pid > 0)
	{
		do {
			waitpid(pid, &(state), WUNTRACED);
		} while (!WIFEXITED(state) && !WIFSIGNALED(state));
		state = WEXITSTATUS(state);
	}
	return (state);
}

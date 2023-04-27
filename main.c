#include "includes/main.h"
#include "includes/string.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

/**
 * execute - execute command
 * @filename: the shell file name
 * @cmd: the command to be executed
 * @environ: the environment variables list
 *
 * Return: execute function state
 */
int execute(char *filename, char *cmd, char **environ)
{
	pid_t pid = 0;
	int state = 0;

	if (!cmd)
		return (-1);

	pid = fork();
	if (!pid)
	{
		char *args[2] = { NULL, NULL };

		args[0] = strncmp(cmd, "./", 2) ? cmd : cmd + 2;
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
	return (0);

}

/**
 * main - simple sh clone
 * @argc: number of arguments
 * @argv: the arguments list
 * @environ: the environment variables list
 *
 * Return: exit state
 */
int main(int argc __attribute__((unused)), char **argv, char **environ)
{
	char *lineptr = NULL, *trimedlineptr;
	int err, exitstate = EXIT_SUCCESS;
	size_t size = 0;

loop:
	show_prompt(stdin);
	err = read_command(stdin, &lineptr, &size, argv[0]);
	if (err < 0)
	{
		if (err == -1)
			exitstate = EXIT_FAILURE;
		goto exit;
	}
	trimedlineptr = _strtrim(lineptr);
	if (execute(argv[0], trimedlineptr, environ))
		goto exit;
	free(lineptr);
	goto loop;
exit:
	if (lineptr)
		free(lineptr);
	return (exitstate);
}

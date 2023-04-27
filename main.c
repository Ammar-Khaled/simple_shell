#include "includes/main.h"
#include <stdlib.h>
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

		args[0] = cmd;
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
	char *lineptr = NULL;
	int err, exitstate = EXIT_SUCCESS;
	size_t size = 0;

loop:
	show_prompt(stdin);
	err = read_command(stdin, &lineptr, &size);
	if (err < 0)
	{
		if (err == READ_FAIL)
		{
			exitstate = EXIT_FAILURE;
			perror("hsh");
		}
		goto exit;
	}
	if (execute(argv[0], lineptr, environ))
		goto exit;
	free(lineptr);
	goto loop;
exit:
	free(lineptr);
	return (exitstate);
}

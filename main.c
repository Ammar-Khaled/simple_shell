#include "includes/main.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

/**
 * execute - execute command
 * @cmd: the command to be executed
 *
 * Return: execute function state
 */
struct execute_state execute(char *cmd)
{
	pid_t pid = -5;
	char *args[2] = { NULL, NULL };
	struct execute_state state = {0, 0};

	args[0] = cmd;

	if (!cmd)
		state.execute = -2;

	if (!state.execute)
		pid = fork();
	if (!pid)
	{
		execve(args[0], args, environ);
		state.execute = -1;
	}
	else if (pid == -1)
		state.execute = -1;
	else if (pid > 0)
	{
		do {
			waitpid(pid, &(state.process), WUNTRACED);
		} while (!WIFEXITED(state.process) && !WIFSIGNALED(state.process));
		state.process = WEXITSTATUS(state.process);
	}
	return (state);
}

/**
 * main - simple sh clone
 *
 * Return: exit state
 */
int main(void)
{
	char *lineptr = NULL;
	size_t line = 0, linesize = 0;
	ssize_t nread = 0;
	struct execute_state state;

loop:
	/* show prompt */
	write(STDIN_FILENO, "$ ", 2);
	line++;
	nread = getline(&lineptr, &linesize, stdin);
	if (nread == -1)
	{
		perror("hsh");
		return (1);
	}
	lineptr[nread - 1] = 0;
	state = execute(lineptr);
	if (state.execute == -1)
		perror("hsh");
	free(lineptr);
	linesize = 0;
	goto loop;

	return (0);
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int execute(const char *name, char *cmd, char *const *environ)
{
	pid_t pid = fork();
	int wstatus;

	if (!pid)
	{
		char *args[2] = { NULL, NULL };

		args[0] = cmd;
		execve(args[0], args, environ);
		perror(name);
		exit(EXIT_FAILURE);
	}
	else if (pid == -1)
	{
		perror(name);
		return (-1);
	}

	do
	{
		waitpid(pid, &wstatus, WUNTRACED);
	} while (!WIFEXITED(wstatus) && !WIFSIGNALED(wstatus));
	return (1);
}

int main(int argc, char **argv, char **environ)
{
	char *name;
	char *lineptr;
	size_t linesize;
	ssize_t nread;

	name = argv[0];
	if (argc > 1)
	{
		execute(name, argv[1], environ);
		exit(0);
	}

begin:
	printf("($) ");
	nread = getline(&lineptr, &linesize, stdin); /* read line */
	if (feof(stdin))
		goto clean;
	if (nread == -1)
	{
		perror(name);
		goto begin;
	}
	lineptr[strlen(lineptr) - 1] = 0; /* remove last byte (delim) */
	execute(name, lineptr, environ);
	goto begin;
clean:
	free(lineptr);
	return (0);
}

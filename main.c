#include "includes/main.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <limits.h>

/**
 * main - simple sh clone
 * @argc: number of arguments
 * @argv: the arguments list
 *
 * Return: exit state
 */
int main(int argc __attribute__((unused)), char **argv)
{
	char *lineptr = NULL, **args = NULL;
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
	args = split_command(lineptr);
	exitstate = execute(argv[0], args);
	free(lineptr);
	lineptr = NULL;
	free(args);
	args = NULL;
	goto loop;
exit:
	if (lineptr)
		free(lineptr);
	if (args)
		free(args);
	return (exitstate);
}

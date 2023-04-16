#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define TOKEN_BUFSIZE 64
#define DELIMETERS " \t\r\a\n"
/**
 * split_line - splits the command line
 * @line: the entire string command line
 *
 * Return: an array of the substrings
 */
char **split_line(char *line)
{
	int token_bufsize, i;
	char *token;
	char **tokens;

	token_bufsize = TOKEN_BUFSIZE;
	i = 0;
	tokens = malloc(token_bufsize * sizeof(char *));
	if (!tokens)
	{
		fprintf(stderr, "Memory allocation error\n");
		exit(EXIT_FAILURE);
	}
	/*get the first token*/
	token = strtok(line, DELIMETERS);
	while (token)
	{
		tokens[i] = token;
		i++;
		/*if needed reallocate the buffer with the size increased by 1KB */
		if (i > token_bufsize - 1)
		{
			token_bufsize += TOKEN_BUFSIZE;
			tokens = realloc(tokens, token_bufsize * sizeof(char *));
			if (!tokens)
			{
				fprintf(stderr, "Memory allocation error\n");
				exit(EXIT_FAILURE);
			}
		}
		/*get the next token*/
		token = strtok(NULL, DELIMETERS);
	}
	tokens[i] = NULL;
	return (tokens);
}

/**
 * execute - executes the exectable program files
 * @name: name of our program (required for error handling)
 * @cmd: array of strings contains the command and its arguments
 * @environ: environment
 *
 * Return: 1 for Success
*/
int execute(const char *name, char **cmd, char *const *environ)
{
	pid_t pid = fork();
	int wstatus;

	if (!pid)
	{
		execve(cmd[0], cmd, environ);
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
	return (1);
}

/**
 * main - simple shell
 * @argc: argument counter
 * @argv: argument vector
 * @environ: environment
 *
 * Return: 0 for success
 */
int main(int argc, char **argv, char **environ)
{
	char *name;
	char *lineptr;
	size_t linesize;
	ssize_t nread;
	char **args;

	name = argv[0];

	/* non-interactive mode: excute one command */
	if (argc > 1)
	{
		execute(name, argv, environ);
		exit(0);
	}

	/* inter-active mode */
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
	args = split_line(lineptr);
	execute(name, args, environ);
	goto begin;
clean:
	free(lineptr);
	return (0);
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include "core.h"
#include "builtin.h"

/**
 * readline - prompt user for the whole command line
 * @prompt: the prompt string printed before every input
 *
 * Return: the string it read
 */
char *readline(const char *prompt)
{
	int bufsize, ibuf, ch;
	char *buf;

	bufsize = BUFSIZE;
	/* allocate one KB for the buffer */
	buf = malloc(bufsize * sizeof(char));
	if (!buf)
		goto end;

	printf("%s", prompt);
	/*read character character until hitting EOF or a new line*/
	while (1)
	{
		ch = getchar();
		if (ch == EOF || ch == '\n')
		{
			buf[ibuf] = '\0';
			return (buf);
		}
		buf[ibuf] = ch;
		ibuf++;
		/*
		 * if the buffer iterator exceeded the buffer size
		 * reallocate the buffer with the size increased by 1KB
		 */
		if (ibuf > bufsize - 1)
		{
			bufsize += BUFSIZE;
			buf = realloc(buf, bufsize * sizeof(char));
			if (!buf)
				goto end;
		}
	}
	return (buf);
end:
	fprintf(stderr, "Memory allocation error\n");
	exit(EXIT_FAILURE);
}

/**
 * splitline - splits the command line
 * @line: the entire string command line
 *
 * Return: an array of the substrings
*/
char **splitline(char *line)
{
	int token_bufsize, i;
	char *token;
	char **tokens;

	token_bufsize = TOKEN_BUFSIZE;
	i = 0;
	tokens = malloc(token_bufsize * sizeof(char *));
	if (!tokens)
		goto end;
	/*get the first token*/
	token = strtok(line, DELIMETERS);
	while (token)
	{
		tokens[i++] = token;
		/*if needed reallocate the buffer with the size increased by 1KB */
		if (i > token_bufsize - 1)
		{
			token_bufsize += TOKEN_BUFSIZE;
			tokens = realloc(tokens, token_bufsize * sizeof(char *));
			if (!tokens)
				goto end;
		}
		/*get the next token*/
		token = strtok(NULL, DELIMETERS);
	}
	tokens[i] = NULL;
	return (tokens);
end:
	fprintf(stderr, "Memory allocation error\n");
	exit(EXIT_FAILURE);
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
 * evaluate - evaluate a command line
 * @name: the shell's name
 * @args: the splitted command line
 * @environ: the environment variables list
 *
 * Return: exit status of the command otherwise -1 on fail
 */
int evaluate(char *name, char **args, char **environ)
{
	int i, argc;
	/* define builtin commands */
	command_t builtins[] = {
		{ "exit", builtin_exit }, { "env", builtin_env },
		{ NULL, NULL }
	};

	/* count number of elements in args */
	for (argc = 0; args[argc]; argc++)
		;

	/* check if command is builtin and execute if exists */
	for (i = 0; builtins[i].name; i++)
		if (!strcmp(builtins[i].name, args[0]))
			return (builtins[i].cmd(argc, args, environ));

	/* if not a builtin command try to execute system executable file */
	return (execute(name, args, environ));
}

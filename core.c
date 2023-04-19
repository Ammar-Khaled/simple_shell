#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "core.h"
#include "builtin.h"
#include "process.h"

/**
 * _strtok - strtok implementation
 * @str: string to split
 * @delim: characters to be used as a delimeter
 *
 * Return: pointer to the start of the token in @str
 */
char *_strtok(char *str, const char *delim)
{
	char *start, *end;
	static char *cache;

	if (str)
		cache = str;
	if (!cache)
		return (NULL);

	start = cache;
	end = strpbrk(cache, delim);
	if (end)
	{
		*end = 0;
		cache = end + 1;
	}
	else
	{
		cache = NULL;
	}
	return (start);
}

/**
 * readline - prompt user for the whole command line
 * @name: the name of the shell
 * @prompt: the prompt string printed before every input
 * @line: pointer to the line cstring variable
 * @size: pointer to the line size variable
 */
void readline(char *name, char *prompt, char **line, size_t *size)
{
	int bufsize = BUFSIZE, ibuf = 0, ch;
	char *buf;

	buf = malloc(bufsize * sizeof(char)); /* allocate one KB for the buffer */
	if (!buf)
		goto end;
	printf("%s", prompt);
	while (1)
	{
		ch = getchar();
		if (ch == EOF)
		{
			if (ibuf)
				continue; /* ignore EOF if buffer is not empty */
			else
				exit(EXIT_SUCCESS); /* exit shell on EOF if buffer is empty */
		}
		buf[ibuf] = ch == '\n' ? 0 : ch;
		if (!buf[ibuf++]) /* end buffer on newline char */
			break;

		/*
		 * if the buffer iterator exceeded the buffer size
		 * reallocate the buffer with the size increased by 1KB
		 */
		if (ibuf >= bufsize)
		{
			bufsize += BUFSIZE;
			buf = realloc(buf, bufsize * sizeof(char));
			if (!buf)
				goto end;
		}
	}
	/* set line to the buffer pointer and set the line size */
	*line = buf, *size = ibuf;
	return;
end:
	fprintf(stderr, "%s: Memory allocation error\n", name);
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
	unsigned int token_bufsize = TOKEN_BUFSIZE, i = 0;
	char *token;
	char **tokens;

	tokens = malloc(token_bufsize * sizeof(char *));
	if (!tokens)
		goto end;
	/*get the first token*/
	token = _strtok(line, DELIMETERS);
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
		token = _strtok(NULL, DELIMETERS);
	}
	tokens[i] = NULL;
	return (tokens);
end:
	fprintf(stderr, "Memory allocation error\n");
	exit(EXIT_FAILURE);
}

/**
 * evaluate - evaluate a command line
 * @name: the shell's name
 * @args: the splitted command line
 *
 * Return: exit status of the command otherwise -1 on fail
 */
int evaluate(char *name, char **args)
{
	int i, argc;
	command_t builtins[] = {
		{"exit", builtin_exit}, {"env", builtin_env},
		{NULL, NULL}
	};

	/* count number of elements in args */
	for (argc = 0; args[argc]; argc++)
		;

	/* check if command is builtin and execute if exists */
	for (i = 0; builtins[i].name; i++)
		if (!strcmp(builtins[i].name, args[0]))
			return (builtins[i].cmd(argc, args));

	/* if not a builtin command try to execute system executable file */
	return (execute(name, args, environ));
}

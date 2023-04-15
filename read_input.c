#include <stdio.h>
#include <stdlib.h>

#define BUFSIZE 1024
#define TOKEN_BUFSIZE 64
#define DELIMETERS " \t\r\a\n"

/**
 * read_line - prompt user for the whole command line
 *
 * Return: the string it read
 */
char *read_line(void)
{
	int bufsize, ibuf, ch;
	char *buf;

	bufsize = BUFSIZE;
	/* allocate one KB for the buffer */
	buf = malloc(bufsize * sizeof(char));
	if (!buf)
	{
		fprintf(stderr, "Memory allocation error\n");
		exit(EXIT_FAILURE);
	}

	printf("> ");
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
			{
				fprintf(stderr, "Memory allocation error\n");
				exit(EXIT_FAILURE);
			}
		}
	}
	return (buf);
}

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
		ffprintf(stderr, "Memory allocation error\n");
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
				ffprintf(stderr, "Memory allocation error\n");
				exit(EXIT_FAILURE);
			}
		}
		/*get the next token*/
		token = strtok(NULL, DELIMETERS);
	}
	tokens[i] = NULL;
	return (tokens);
}

#include "includes/main.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

/**
 * show_prompt - show prompt only if the stream is a tty
 * @ctx: the command line context
 */
void show_prompt(context *ctx)
{
	int fd = fileno(ctx->stream);

	if (isatty(fd))
		write(fd, "$ ", 2);
}

/**
 * read_command - read user input
 * @ctx: the command line context
 */
void read_command(context *ctx)
{
	size_t linesize = 0;
	ssize_t nread = 0;

	nread = getline(&ctx->lineptr, &linesize, ctx->stream);
	if (nread > 0)
		ctx->lineptr[nread - 1] = 0;
	else if (errno == EINVAL || errno == ENOMEM)
	{
		perror(ctx->shell_name);
		ctx->signal = S_FAIL;
	}
	else
	{
		ctx->signal = S_EXIT;
	}
}

/**
 * split_command - splits the input line into tokens to execute
 * @ctx: the context line object
 */
void split_command(context *ctx)
{
	char *token, *trimed;
	size_t count = 0, size = 5;

	if (!ctx->lineptr)
		goto fail;

	reset_state(ctx);
	ctx->args = malloc(size * sizeof(char *));
	if (!ctx->args)
		goto fail;

	trimed = _strtrim(ctx->lineptr);
	token = _strtok(trimed, " \n\r\t\v");
	while (token)
	{
		ctx->args[count++] = token;
		if (count == size)
		{
			ctx->args = _realloc(ctx->args,
				size * sizeof(char *), (size + 5) * sizeof(char *));
			size += 5;
			if (!ctx->args)
				goto fail;
		}
		token = _strtok(NULL, " \n\r\t\v");
	}
	ctx->argc = count;
	ctx->args[count] = NULL;
	return;
fail:
	ctx->signal = S_FAIL;
}

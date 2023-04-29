#include "includes/main.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <limits.h>

/**
 * free_context - free the allocated data pointed inside the command line ctx
 * @ctx: the command line context
 */
void free_context(context *ctx)
{
	if (ctx->lineptr)
		free(ctx->lineptr);
	if (ctx->args)
		free(ctx->args);
	ctx->signal = S_NULL;
	ctx->lineptr = NULL;
	ctx->args = NULL;
}

/**
 * main - simple sh clone
 * @argc: number of arguments
 * @argv: the arguments list
 *
 * Return: exit state
 */
int main(int argc __attribute__((unused)), char **argv)
{
  /* stream, lineptr, shell_name, args, cmd_name, argc, state, line, signal */
	context ctx = { stdin, NULL, NULL, NULL, NULL, 0, EXIT_SUCCESS,	0, S_NULL };

	ctx.shell_name = argv[0];
loop:
	show_prompt(&ctx);
	ctx.line++;
	read_command(&ctx);
	if (ctx.signal == S_EXIT)
		goto exit;
	else if (ctx.signal == S_FAIL)
		goto loop;
	split_command(&ctx);
	if (ctx.signal == S_NULL)
		execute(&ctx);
	if (ctx.signal == S_EXIT)
		goto exit;
	free_context(&ctx);
	goto loop;
exit:
	free_context(&ctx);
	return (ctx.state);
}

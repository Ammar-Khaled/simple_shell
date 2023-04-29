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
	ctx->lineptr = NULL;
	ctx->args = NULL;
}

/**
 * solve_args_envars - replace envar args with its values
 * @ctx: the context object
 */
void solve_args_envars(context *ctx)
{
	int i;
	char *value;

	for (i = 0; ctx->args[i]; i++)
		if (!strncmp("$", ctx->args[i], 1))
		{
			if (!strlen(ctx->args[i] + 1))
				return;
			value = getenv(ctx->args[i] + 1);
			ctx->args[i] = value ? value : "";
		}
}

/**
 * reset_state - resets the current context state
 * @ctx: the context object
 */
void reset_state(context *ctx)
{
	ctx->signal = S_NULL;
	ctx->state = EXIT_SUCCESS;
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
  /*
	 * stream, lineptr, shell_name, args, cmd_name, argc, state, line, signal,
	 * mode
	 */
	context ctx = { stdin, NULL, NULL, NULL, NULL, 0, EXIT_SUCCESS,	0, S_NULL,
		M_INIT };

	ctx.shell_name = argv[0];
	setenv("?", "0", 1);
loop:
	show_prompt(&ctx);
	ctx.line++;
	read_command(&ctx);
	if (ctx.signal == S_EXIT)
		goto exit;
	else if (ctx.signal == S_FAIL)
		goto loop;
	split_command(&ctx);
	if (ctx.signal == S_NULL && ctx.argc > 0)
		execute(&ctx);
	/* if signal is exit the exit */
	if (ctx.signal == S_EXIT)
		goto exit;
	/* if not interactive mode and has fail state then exit */
	if (!isatty(fileno(ctx.stream)) && ctx.state)
		goto exit;
	free_context(&ctx);
	goto loop;
exit:
	free_context(&ctx);
	return (ctx.state);
}

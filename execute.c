#include "includes/main.h"
#include <limits.h>
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <dirent.h>
#include <unistd.h>

/**
 * cmd_get_path - search and get the command path
 * @cmd: the command to look for
 *
 * Return: allocated command fullpath otherwise NULL
 */
char *cmd_get_path(char *cmd)
{
	char *token, *resolved, fullpath[PATH_MAX], *path;

	if (!cmd)
		return (NULL);
	resolved = realpath(cmd, NULL);
	if (resolved)
		return (resolved);
	path = getenv("PATH");
	if (!path)
		return (NULL);
	path = strdup(path);
	if (!path)
		return (NULL);

	token = strtok(path, ":");
	while (token)
	{
		snprintf(fullpath, PATH_MAX, "%s/%s", token, cmd);
		resolved = realpath(fullpath, NULL);
		if (resolved)
			goto found;
		token = strtok(NULL, ":");
	}
	free(path);
	return (NULL);
found:
	free(path);
	return (resolved);
}

/**
 * get_builtin - searches and returns the action of the builtin command
 * @name: the name of the builtin command to look for
 *
 * Return: the command's action if command found otherwise NULL
 */
builtin_action get_builtin(char *name)
{
	int i;
	builtin builtins[] = {
		{ "exit", builtin_exit },
	/*
	 * { "env", builtin_env }, { "cd", builtin_cd },
	 *	{ "setenv", builtin_setenv }, { "unsetenv", builtin_unsetenv },
	 */
		{ NULL, NULL }
	};

	for (i = 0; builtins[i].name; i++)
		if (!strcmp(builtins[i].name, name))
			return (builtins[i].action);
	return (NULL);
}

/**
 * print_error - prints error to stderr
 * @ctx: the command line context
 * @err: the error message
 */
void print_error(context *ctx, char *err)
{
	fprintf(stderr, "%s: %d: %s: %s\n",
				 ctx->shell_name, ctx->line, ctx->cmd_name, err);
}

/**
 * execute - execute command
 * @ctx: the command line context
 */
void execute(context *ctx)
{
	char *cmd_path;
	builtin_action action;

	reset_state(ctx);
	if (!ctx->args || !*(ctx->args) || !environ)
		goto fail;

	ctx->cmd_name = ctx->args[0];
	action = get_builtin(ctx->cmd_name);
	if (action)
	{
		ctx->state = action(ctx);
		return;
	}

	cmd_path = cmd_get_path(ctx->cmd_name);
	if (cmd_path)
	{
		run_command(ctx, cmd_path);
		free(cmd_path);
		return;
	}
	else
		print_error(ctx, "not found");
fail:
	ctx->signal = S_FAIL;
	ctx->state = EXIT_FAILURE;
}

/**
 * run_command - execute system command
 * @ctx: the command line context
 * @cmd_path: the system command fullpath
 */
void run_command(context *ctx, char *cmd_path)
{
	pid_t pid = fork();

	if (!pid)
	{
		execve(cmd_path, ctx->args, environ);
		perror(ctx->shell_name);
		ctx->state = EXIT_FAILURE;
		ctx->signal = S_EXIT;
	}
	else if (pid == -1)
	{
		perror(ctx->shell_name);
		ctx->signal = S_FAIL;
	}
	else if (pid > 0)
	{
		do {
			waitpid(pid, &(ctx->state), WUNTRACED);
		} while (!WIFEXITED(ctx->state) && !WIFSIGNALED(ctx->state));
		ctx->state = WEXITSTATUS(ctx->state);
	}
}

#ifndef __MAIN_H__

#define __MAIN_H__
#include <stdio.h>

#define ERR_NOT_FOUND 127
#define ERR_EXIT_INVALID_ARG 128
#define ERR_MISUSE_BUILTIN 2

/**
 * enum context_signal - command processing end signal type
 * @S_NULL: no action to take
 * @S_EXIT: free and exit the shell
 * @S_FAIL: command processing failed
 */
enum context_signal
{ S_NULL, S_EXIT, S_FAIL };

/**
 * enum run_mode - the mode that the shell is currently running in
 * @M_INIT: initial value before shell setup determines the running mode
 * @M_TTY: the input stream is a tty
 * @M_NTTY: the input stream is not a tty
 * @M_FILE: the opened stream is a commands file
 */
enum run_mode
{ M_INIT, M_TTY, M_NTTY, M_FILE };

extern char **environ;

/**
 * struct context_s - object to hold the line state
 * @stream: the opened stream for command reading
 * @lineptr: the pointer for the user input
 * @shell_name: the name of the hsh executable file
 * @args: the current command arguments
 * @cmd_name: the current command name as inputed
 * @argc: number of arguments
 * @state: the current exit state
 * @line: the current command line number
 * @signal: the command processing end signal
 * @mode: the currently running mode
 */
typedef struct context_s
{
	FILE *stream;
	char *lineptr;
	char *shell_name;
	char **args;
	char *cmd_name;
	int argc;
	int state;
	int line;
	enum context_signal signal;
	enum run_mode mode;
} context;

typedef int (*builtin_action)(context *ctx);

/**
 * struct builtin_s - builtin command definition
 * @name: command name
 * @action: command action function
 */
typedef struct builtin_s
{
	char *name;
	builtin_action action;
} builtin;

/* main.c */
void free_context(context *ctx);
void reset_state(context *ctx);
void solve_args_envars(context *ctx);

/* execute.c */
char *cmd_get_path(char *cmd);
builtin_action get_builtin(char *name);
void print_error(context *ctx, char *err, char *arg);
void execute(context *ctx);
void run_command(context *ctx, char *cmd_path);

/* input.c */
void show_prompt(context *ctx);
void read_command(context *ctx);
void split_command(context *ctx);

/* builtin.c */
int builtin_exit(context *ctx);
int builtin_cd(context *ctx);
int builtin_env(context *ctx);
int builtin_setenv(context *ctx);
int builtin_unsetenv(context *ctx);

/* memory.c */
int min(int a, int b);
void *_memset(void *ptr, char byte, unsigned int nbytes);
void *_memcpy(void *dest, void *src, unsigned int nbytes);
void *_realloc(void *ptr, unsigned int old_size, unsigned int new_size);

/* string.c */
char *_strtrim(char *str);
char *_strtok(char *str, char *delim);

#endif

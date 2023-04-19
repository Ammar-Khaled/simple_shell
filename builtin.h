#ifndef __BUILTIN_H__
#define __BUILTIN_H__

/**
 * struct command_s - structure to hold the buildin commands
 * @name: the command name
 * @cmd: the command function to be executed
 */
typedef struct command_s
{
	char *name;
	int (*cmd)(int argc, char **argv);
} command_t;

int builtin_exit(int argc, char **argv);
int builtin_env(int argc, char **argv);

/* define environment variables external variable */
extern char **environ;

#endif

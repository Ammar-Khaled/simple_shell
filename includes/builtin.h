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

/**
 * Alias - struct for an alias
 * @alias: the alias name
 * @command: the actual corresponding command
 */
typedef struct alias
{
	char *alias_name;
	char *command;
	struct alias *next;
} Alias;

int builtin_exit(int argc, char **argv);
int builtin_env(int argc, char **argv);
int builtin_cd(int argc, char **argv);
int builtin_setenv(int argc, char **argv);
int builtin_unsetenv(int argc, char **argv);
int builtin_alias(int argc, char **argv);

/* define environment variables external variable */
extern char **environ;

#endif

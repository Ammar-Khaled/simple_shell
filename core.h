#ifndef __MAIN_H__
#define __MAIN_H__
#include <stddef.h>

#define BUFSIZE 1024
#define TOKEN_BUFSIZE 64
#define DELIMETERS " \t\r\a\n"

typedef int (*command_fn)(int argc, char **argv, char **environ);
/**
 * struct command_s - structure to hold the buildin commands
 * @name: the command name
 * @cmd: the command function to be executed
 */
typedef struct command_s
{
	char *name;
	command_fn cmd;
} command_t;

char *_strtok(char *str, const char *delim);
void readline(char *name, char *prompt, char **line, size_t *size);
char **splitline(char *line);
int evaluate(char *name, char **args, char **environ);

#endif

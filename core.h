#ifndef __MAIN_H__
#define __MAIN_H__
#include <stddef.h>

#define BUFSIZE 1024
#define TOKEN_BUFSIZE 64
#define DELIMETERS " \t\r\a\n"

extern char **environ;

char *_strtok(char *str, const char *delim);
void readline(char *prompt, char **line, size_t *size);
char **splitline(char *line);
int evaluate(char **args);

#endif

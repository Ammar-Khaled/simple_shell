#ifndef __UTILS_S__
#define __UTILS_S__

#include <stddef.h>
extern char **environ;

char *_strchr(char *str, char c);
size_t _strspn(char *str, char *delim);
char *_strtok(char *str, char *delim);

#endif

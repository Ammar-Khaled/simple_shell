#ifndef __UTILS_S__
#define __UTILS_S__

#include <stddef.h>
extern char **environ;

size_t _strlen(char *str);
int _strcmp(char *s1, char *s2);
char *_strchr(char *str, char c);
size_t _strspn(char *str, char *delim);
char *_strtok(char *str, char *delim);

char *_strdup(char *str);
char *_strcpy(char *dest, char *src);
int _strncmp(char *s1, char *s2, size_t nbytes);

#endif

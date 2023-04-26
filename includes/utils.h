#ifndef __UTILS_S__
#define __UTILS_S__

#include <stddef.h>
#include <unistd.h>
extern char **environ;

size_t _strlen(char *str);
int _strcmp(char *s1, char *s2);
char *_strchr(char *str, char c);
size_t _strspn(char *str, char *delim);
char *_strtok(char *str, char *delim);

char *_strdup(char *str);
char *_strcpy(char *dest, char *src);
int _strncmp(char *s1, char *s2, size_t nbytes);
void _strrev(char *str, int len);

ssize_t print(int fd, char *str);
ssize_t printo(char *str);
ssize_t printe(char *str);
ssize_t printu(int fd, unsigned long num);
ssize_t printuo(unsigned long num);
int number_converter(char *str, unsigned long value, int base);

void *_realloc(void *ptr, unsigned int old_size, unsigned int new_size);
int min(int a, int b);

#endif

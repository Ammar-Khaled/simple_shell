#ifndef __ENV_H__
#define __ENV_H__

#include "envar.h"

int envman_init(char **environ);
envar envman_global(envar var, char reset);
void envman_destroy(void);
envar envman_loop(char rewind);

envar envman_search(char *name);
char *envman_value(char *name);
int envman_set(char *name, char *value);

#endif

#ifndef __PROCESS_H__
#define __PROCESS_H__

#define PATH_DELIM ":"
#define FULLPATH_MAX 1024

int execute(const char *name, char **cmd, char *const *environ);

#endif

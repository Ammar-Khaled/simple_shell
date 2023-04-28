#ifndef __MAIN_H__

#define __MAIN_H__
#include <stdio.h>

#define READ_END -2
#define READ_FAIL -1

/**
 * struct execute_state - the state information of the execute function
 * @process: the state of the executed process only set if @execute is zero
 * @execute: the state of the execute function itself -ve value means failure
 */
struct execute_state
{
	int process;
	int execute;
};

int execute(char *filename, char **args, char **environ);

void show_prompt(FILE *stream);
int read_command(FILE *stream, char **lineptr, size_t *size, char *filename);
char **split_command(char *lineptr);

#endif

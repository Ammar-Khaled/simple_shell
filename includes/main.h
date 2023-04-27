#ifndef __MAIN_H__

#define __MAIN_H__
extern char **environ;

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

struct execute_state execute(char *cmd);

#endif

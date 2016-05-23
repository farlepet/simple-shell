#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#include "simple-shell.h"

char *_split_cmd_args[CMD_MAX_ARGS + 1] = { NULL };

char **split_cmd(char *cmd, int *n_args) {
	*n_args = 1;
	_split_cmd_args[0] = cmd;
	int i = 0, slen = strlen(cmd);
	char quote_ch = 0;
	for(; i < slen; i++) {
		if(*n_args > CMD_MAX_ARGS) {
			printf("WARNING: Exeeded maximum number of allowable arguments!\n");
			return _split_cmd_args;
		}
		char ch = cmd[i];
		if(ch == quote_ch) {
			quote_ch = 0;
			memcpy(cmd + i, cmd + (i+1), slen - i);
			slen--;
			i--;
			continue;
		}
		else if(ch == '"' || ch == '\'') {
			quote_ch = ch;
			memcpy(cmd + i, cmd + (i+1), slen - i);
			slen--;
			i--;
			continue;
		}
		
		if(ch == ' ') {
			if(quote_ch == 0) {
				cmd[i] = '\0';
				_split_cmd_args[*n_args] = cmd + (i+1);
				(*n_args)++;
			}
			continue;
		}
	}
	_split_cmd_args[*n_args] = NULL;
	return _split_cmd_args;
}

int run_command(char *cmd) {
	int n_args, pid, ret;
	char **args = split_cmd(cmd, &n_args);
	
	#if DEBUG == 1
	int i;
	for(i = 0; i < n_args; i++) {
		printf("args[%d]: %s\n", i, args[i]);
	}
	#endif
	
	// Check for built-ins:
	if(!strcmp(args[0], "cd")) {
		return chdir(args[1]);
	} else if (!strcmp(args[0], "exit")) {
		exit(0);
	}
	
	pid = fork();
	if(pid == 0) {
		execvp(args[0], args);
	}
	wait(&ret);
	
	return ret;
}
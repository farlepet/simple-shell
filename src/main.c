#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#define MAX_LINE_LEN 65535
#define CMD_MAX_ARGS 512
#define CWD_MAX_LEN 128
char cmd_line[MAX_LINE_LEN+1];
char *_split_cmd_args[CMD_MAX_ARGS + 1] = { NULL };
char cwd_str[CWD_MAX_LEN+1];


#define DEBUG 0

int run_command(char *cmd);
char **split_cmd(char *cmd, int *n_args);

int main(int argc, char **argv) {
	int i = 1; // Skip program name
	for(; i < argc; i++) {
		if(argv[i][0] == '-') {
			switch(argv[i][1]) {
				case 'c': // Run a command
					return run_command(argv[i+1]);
				
				default:
					printf("Unrecognized command: %s\n", argv[i]);
					return 1;
			}
		}
		else {
			printf("Unrecognized command: %s\n", argv[i]);
			return 1;
		}
	}
	
	while(1) {
		getcwd(cwd_str, CWD_MAX_LEN);
		printf("[%s]> ", cwd_str);
		fgets(cmd_line, MAX_LINE_LEN, stdin);
		if(cmd_line[strlen(cmd_line) - 1] == '\n') cmd_line[strlen(cmd_line) - 1] = '\0';
#if DEBUG == 1
		printf("[%s]\n", cmd_line);
#endif
		run_command(cmd_line);
	}
	
	return 0; // TODO: Make a shell!
}

int run_command(char *cmd) {
	int n_args, pid;
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
	}
	pid = fork();
	if(pid == 0) {
		execvp(args[0], args);
	}
	wait(NULL);
	
	return 0;
}

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
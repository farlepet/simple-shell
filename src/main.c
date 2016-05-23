#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#include "simple-shell.h"

char cmd_line[MAX_LINE_LEN+1];
char cwd_str[CWD_MAX_LEN+1];




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
	
	int ret = 0;
	while(1) {
		getcwd(cwd_str, CWD_MAX_LEN);
		printf("[%d]<%s> ", ret, cwd_str);
		fgets(cmd_line, MAX_LINE_LEN, stdin);
		if(cmd_line[strlen(cmd_line) - 1] == '\n') cmd_line[strlen(cmd_line) - 1] = '\0';
#if DEBUG == 1
		printf("[%s]\n", cmd_line);
#endif
		ret = run_command(cmd_line);
	}
}
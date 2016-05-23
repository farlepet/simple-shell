
#define DEBUG 0

#define MAX_LINE_LEN 65535
#define CWD_MAX_LEN 128
#define CMD_MAX_ARGS 512

int run_command(char *cmd);
char **split_cmd(char *cmd, int *n_args);
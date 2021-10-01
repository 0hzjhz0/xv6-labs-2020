#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

#define STDIN_FILENO 0
#define MAXBUF 1024

int main(int argc, char *argv[])
{
	if (argc < 2) {
		fprintf(2, "usage: xargs <your_command>\n");
		exit(1);
	}

	if (argc + 1 > MAXARG) {
		fprintf(2, "too many arguments\n");
		exit(1);
	}

	char *cmd = argv[1];  // cmd to exec

	int args_idx = 0;
	char *params[MAXARG];
	for (int i = 1; i < argc; i++) params[args_idx++] = argv[i];
	
	int n = 0;
	char buf[MAXBUF];
	while((n = read(STDIN_FILENO, buf, MAXBUF)) > 0) {
		if (fork() == 0) {
			char *arg = (char *) malloc(sizeof(buf));
			int idx = 0;
			for (int i = 0; i < n; i++) {
				if (buf[i] == ' ' || buf[i] == '\n') {
					arg[idx] = 0;
					params[args_idx++] = arg;
					idx = 0;
					arg = (char *) malloc(sizeof(buf));
				} else {
					arg[idx++] = buf[i];
				}
			}
			arg[idx] = 0;
			params[args_idx] = 0;
			exec(cmd, params);
		} else {
			wait(0);
		}
		
	}
	exit(0);
}

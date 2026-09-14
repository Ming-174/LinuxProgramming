#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>
int main(int argc,char* argv[]) {
	if (argc < 2) {
		fprintf(stdout, "Error using!\n");
		return 1;
	}

	pid_t pid = fork();
	if (pid == 0) {
		execvp(argv[1], &argv[1]);
		perror("execvp ERROR");
		_exit(127);
	}
	if (pid < 0) {
		perror("Fail to create process");
		return 1;
	}

	int status;
	pid_t ret = waitpid(pid, &status, 0);
	if (ret == -1 ) {
		perror("waitpid Error");
		return 1;
	}
	if (WIFEXITED(status)) {
		fprintf(stderr, "Exit code:%d\n", WEXITSTATUS(status));
	}
	else if (WIFSIGNALED(status)) {
		fprintf(stderr, "Signal %d\n", WTERMSIG(status));
	}

	return 0;
}
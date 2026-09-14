#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<string.h>
int main() {
	int fd[2];
	if (pipe(fd) == -1) {
		perror("Fail to pipe");
		return 1;
	}

	char buf[200] = { 0 };

	pid_t Beth = fork();
	if (Beth == 0) {
		close(fd[1]);
		ssize_t len = read(fd[0], buf, sizeof(buf));
		if (len < 0) {
			perror("Beth read ERROR");
			return 1;
		}
		fprintf(stdout, "Beth recive:%s", buf);
		_exit(0);
	}
	else {
		close(fd[0]);
		fprintf(stdout, "Beth,i do very like you\n");
		sprintf(buf, "Beth%d really like you too!\n", Beth);
		ssize_t ret = write(fd[1], buf, strlen(buf) + 1);
		int status;
		int w = waitpid(Beth, &status, 0);
		printf("Beth's status:%d\n", WEXITSTATUS(status));
	}
	return 0;
}
#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<sys/wait.h>
#include<sys/types.h>
int main() {
	int fd[2];
	if (pipe(fd) == -1) {
		perror("Pipe ERROR");
		return 1;
	}

	pid_t Harmon = fork();
	if (Harmon == -1) {
		perror("Fork ERROR");
		return 1;
	}

	if (Harmon == 0) {
		close(fd[1]);
		dup2(fd[0], 0);
		close(fd[0]);
		execlp("grep", "grep", "Beth Harmon", NULL);
		perror("Fail to exec");
		_exit(1);
	}
	else {
		close(fd[0]);
		char buf1[] = "Hi this is Beth Harmon!\nHaven't see you for a very long time\n";
		char buf2[] = "But i want to tell you\nBeth Harmon love you\n";
		int ret1 = write(fd[1], buf1, strlen(buf1));
		if (ret1 == -1) {
			fprintf(stderr, "First write ERROR\n");
			return 1;
		}
		int ret2 = write(fd[1], buf2, strlen(buf2));
		if (ret2 == -1) {
			fprintf(stderr, "Second write ERROR\n");
			return 1;
		}

		int status;
		close(fd[1]);
		int w = waitpid(Harmon, &status, 0);
		
	}
	return 0;
}
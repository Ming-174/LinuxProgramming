#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<string.h>
int main() {
	int fd[2];
	if (pipe(fd) == -1) {
		perror("Pipe ERROR");
		return 1;
	}

	pid_t Beth = fork();
	if (Beth == 0) {
		close(fd[1]);
		dup2(fd[0], 0);
		close(fd[0]);
		execlp("grep", "grep", "txt", NULL);
		perror("Beth ERROR");
		_exit(1);
	}

	pid_t Harmon = fork();
	if (Harmon == 0) {
		close(fd[0]);
		dup2(fd[1], 1);
		close(fd[1]);
		execlp("ls", "ls", "-l", NULL);
		perror("Harmon ERROR");
		_exit(1);
	}

	close(fd[1]);
	close(fd[0]);
	int status1, status2;
	int w1 = waitpid(Beth, &status1, 0);
	int w2 = waitpid(Harmon, &status2, 0);

	return 0;
}
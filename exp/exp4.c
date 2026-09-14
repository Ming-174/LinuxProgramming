#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
int main() {
	printf("Before fork,pid:%d\n", getpid());

	pid_t pid = fork();
	int status;
	if (pid == 0) {
		printf("Child is running,pid:%d\n", getpid());
		_exit(0);
	}
	else {
		printf("Parent is running,pid:%d\n", getpid());
		printf("Now we close %d\n", pid);
		sleep(10);
		pid_t ret = wait(&status);
		printf("wait return %d\n", ret);
		printf("WIFEXITED=%d\n", WIFEXITED(status));
		printf("WEXITSTATUS=%d\n", WEXITSTATUS(status));
	}
	return 0;
}
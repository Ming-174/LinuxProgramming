#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
int main() {
	printf("Before fork\n");
	pid_t pid = fork();
	printf("Son's pid:%d\n", pid);
	if (pid == 0) {
		printf("Son is running,pid:%d\n", getpid());
		_exit(0);
	}
	else {
		printf("Father is running,pid:%d\n", getpid());
		printf("In this second,father is sleep,type\"ps aux | grep Z\"to check Zomb\n");
		sleep(10);
		printf("Father is over\n");
	}
	return 0;
}
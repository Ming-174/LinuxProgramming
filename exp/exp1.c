#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
int main() {
	printf("Before fork,my pid is %d\n", getpid());

	pid_t pid = fork();

	printf("After fork,my pid is %d ,the son's pid is %d\n", getpid(), pid);
	return 0;
}
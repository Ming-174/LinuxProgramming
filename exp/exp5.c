#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>
int main() {
	pid_t juno = fork();
	if (juno == 0) {
		printf("This is juno\n");
		sleep(3);
		_exit(11);
	}

	pid_t beth = fork();
	if (beth == 0) {
		printf("This is beth\n");
		sleep(6);
		_exit(22);
	}

	int status;
	int ret1 = waitpid(juno, &status, 0);
	printf("Juno's pid is %d ,status: %d\n", juno, WEXITSTATUS(status));

	int ret2 = waitpid(beth, &status, WNOHANG);
	if (ret2 == 0)printf("Beth like you\n");
	printf("Status is %d\n", status);

	ret2 = waitpid(beth, &status, 0);
	printf("Beth's pid is %d ,status: %d", beth, WEXITSTATUS(status));

	return 0;
}
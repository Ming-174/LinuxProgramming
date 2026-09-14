#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
int main() {
	pid_t pid = fork();
	//如果子进程先执行，他会变成僵尸，sleep的这10秒可以检查
	//如果父进程先执行，sleep这十秒会将子进程提入队列运行，从而变成僵尸，几乎是一瞬间的事情，而我们在这时候可以检查僵尸
	if (pid == 0) {
		printf("Son is created\n");
		_exit(0);
	}
	else {
		printf("Father is sleeping\n");
		sleep(10);
		printf("Father wake!\n");
	}
	return 0;
	
}
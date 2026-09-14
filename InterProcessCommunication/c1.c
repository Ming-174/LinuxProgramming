#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
int main() {
	const char* fifo1 = "/home/ming174/code707/ipc/FIFO1";
	const char* fifo2 = "/home/ming174/code707/ipc/FIFO2";
	mkfifo(fifo1, 0666);
	mkfifo(fifo2, 0666);
	char buf[2][200] = {
		"Hi Beth!",
		"I really like you ,Beth"
	};
	int fd1 = open(fifo1, O_WRONLY);
	int fd2 = open(fifo2, O_RDONLY);

	for (int i = 0; i < 2; i++) {
		int len = sizeof(buf[i]);
		write(fd1, &len, sizeof(len));
		write(fd1, buf[i], sizeof(buf[i]));
		char tmp[200] = { 0 };
		read(fd2, &len, sizeof(len));
		read(fd2, tmp, len);
		printf("%s\n", tmp);
		sleep(1);
	}
	return 0;
}
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
		"Hi!",
		"I LOVE YOU"
	};
	int fd1 = open(fifo1, O_RDONLY);
	int fd2 = open(fifo2, O_WRONLY);

	for (int i = 0; i < 2; i++) {
		int len;
		char tmp[200] = { 0 };
		read(fd1, &len, sizeof(len));
		read(fd1, tmp, len);
		printf("%s\n", tmp);
		sleep(2);
		len = sizeof(buf[i]);
		write(fd2, &len, sizeof(len));
		write(fd2, buf[i], sizeof(buf[i]));
	}
	return 0;
}
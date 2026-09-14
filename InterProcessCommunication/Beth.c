#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
int main() {
	char fifo[] = "/home/ming174/code707/ipc/FIFO";
	mkfifo(fifo, 0666);
	int fd = open(fifo, O_WRONLY);
	if (fd < 0) {
		perror("Write fifo open ERROR");
		return 1;
	}
	char buf[] = "Beth love you";
	write(fd, buf, sizeof(buf));
	close(fd);
	return 0;
}
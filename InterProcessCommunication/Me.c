#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/stat.h>
int main() {
	const char* fifo = "/home/ming174/code707/ipc/FIFO";
	mkfifo(fifo, 0666);
	int fd = open(fifo, O_RDONLY);
	if (fd < 0) {
		perror("Read fifo open ERROR");
		return 1;
	}
	char buf[200] = { 0 };
	read(fd, buf, sizeof(buf));
	printf("Hi Beth!\n");
	printf("%s\n", buf);
	return 0;
}
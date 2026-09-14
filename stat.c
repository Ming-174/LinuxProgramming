#include<sys/types.h>
#include<sys/stat.h>
#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
int main() {
	struct stat buf;
	stat("my_link", &buf);
	printf("stat( ) = size:%ld  mod:%o\n ", (long)buf.st_size, buf.st_mode);

	lstat("my_link", &buf);
	printf("lstat( ) = size:%ld  mod=%o\n", (long)buf.st_size, buf.st_mode);

	int fd = open("tmp.c", O_RDONLY);
	if (fd > 0) {
		fstat(fd, &buf);
		printf("fstat( ) = size:%ld  mod=%o\n", buf.st_size, buf.st_mode);
		close(fd);
	}
}
#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/types.h>
int main(int argc,char* argv[]) {
	int fd1 = open(argv[1], O_RDONLY);
	if (fd1 < 0) {
		perror("Fail to open first file");
		return 1;
	}
	int fd2 = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd2 < 0) {
		perror("Fail to open second file");
		close(fd1);
		return 1;
	}
	char buf[1024];
	ssize_t n;
	while ((n = read(fd1, buf, sizeof(buf))) > 0) {
		ssize_t written = 0;
		while (n > written) {
			ssize_t ret = write(fd2, buf + written, n - written);
			if (ret < 0) {
				perror("ERROR WRITING");
				close(fd1);
				close(fd2);
				return 1;
			}
			written += ret;
		}
	}
	close(fd1);
	close(fd2);
	return 0;
}
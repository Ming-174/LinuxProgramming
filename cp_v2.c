#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
int main(int argc, char* argv[]) {
	int fd1 = open(argv[1], O_RDONLY);
	if (fd1 < 0)return -1;
	int fd2 = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd2 < 0) {
		close(fd1);
		return -1;
	}
	char buf[1024];
	int n;
	while ((n = read(fd1, buf, sizeof(buf)) )> 0) {
		int written = 0;
		while (written < n) {
			//如果write写入中断，就反复重写
			int ret = write(fd2, buf + written, n - written);
			written += ret;
		}
	}
	close(fd1);
	close(fd2);
	return -1;
}

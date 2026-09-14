#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#define LENGTH 2000
int main() {
	int fd = open("/usr/include/gnu-versions.h", O_RDONLY);
	if (fd < 0) {
		perror("Fail to open the file");
		return 1;
	}
	char buf[LENGTH];
	//ssize_t by = read(fd, buf, LENGTH);
	//if (by < 0) {
	//	perror("Fail to read the file");
	//	return 1;
	//}
	//else if (by == 0) {
	//	printf("The file is empty\n");
	//	close(fd);
	//	return 0;
	//}
	//for (ssize_t i = 0; i < by; i++) {
	//	putchar(buf[i]);
	//}
	ssize_t n;
	while ((n = read(fd, buf, LENGTH)) > 0) {
		ssize_t ret = write(STDOUT_FILENO, buf, n);
		if (ret < 0) {
			perror("Fail to print");
			close(fd);
			return 1;
		}
	}
	close(fd);
	return 0;
}
#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
int main() {
	const char* filename = "Hello.txt";
	int fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0644);
	char data[] = "HelloLinux";
	write(fd, data, strlen(data));

	lseek(fd, 0, SEEK_SET);
	char buf[1024] = { 0 };
	read(fd, buf, 5);
	printf("从基准SEEK_SET位置开始的5个字符是%s\n", buf);
	memset(buf, 0, 1024);
	
	lseek(fd, 0, SEEK_CUR);
	read(fd, buf, 5);
	printf("从基准SEEK_CUR位置开始的5个字符是%s\n", buf);
	memset(buf, 0, 1024);

	lseek(fd, -10, SEEK_END);
	read(fd, buf, 10);
	printf("从基准SEEK_END位置偏移-10位置的10个字符是%s\n", buf);
	close(fd);
	return 0;
}
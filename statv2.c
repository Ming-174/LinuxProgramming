#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<stdlib.h>
int main() {
	int fd;
	const char* f_path = "test1";
	mode_t f_mode = 0644;
	fd = open(f_path, O_WRONLY | O_CREAT | O_TRUNC, f_mode);
	if (fd < 0) {
		puts("fail to create");
		return 1;
	}
	else puts("file created");
	struct stat* buf = malloc(sizeof(struct stat));
	if (buf == NULL) {
		puts("fail to malloc");
		close(fd);
		return 1;
	}
	int fs = fstat(fd, buf);
	if (fs == -1) {
		puts("fail to fstat");
		close(fd);
		free(buf);
		return 1;
	}
	printf("File mode = %o\n", buf->st_mode);
	close(fd);
	free(buf);
	return 0;
}

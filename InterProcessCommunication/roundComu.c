#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>
int main() {
	int fd[2];
	if (pipe(fd) == -1) {
		perror("Pipe ERROR");
		return 1;
	}

	pid_t Beth = fork();
	if (Beth == 0) {
		close(fd[1]);
		for (int i = 0; i < 4; i++) {
			int len;
			char buf[200] = { 0 };
			int r1 = read(fd[0], &len, sizeof(len));
			if (r1 < 1) {
				perror("Read ERROR Length");
				return 1;
			}
			int r2 = read(fd[0], buf, len);
			if (r2 < len) {
				perror("Read lost");
				return 1;
			}
			fprintf(stdout, "%s", buf);
		}
		fprintf(stdout, "Beth Harmon love you\n");
		_exit(0);
	}
	else {
		close(fd[0]);
		char buf1[] = "Beth\n";
		char buf2[] = "Harmon\n";
		char buf3[] = "Love\n";
		char buf4[] = "You\n";
		int len;
		len = sizeof(buf1);
		write(fd[1], &len, sizeof(len));
		write(fd[1], buf1, sizeof(buf1));
		sleep(1);
		len = sizeof(buf2);
		write(fd[1], &len, sizeof(len));
		write(fd[1], buf2, sizeof(buf2));
		sleep(1);
		len = sizeof(buf3);
		write(fd[1], &len, sizeof(len));
		write(fd[1], buf3, sizeof(buf3));
		sleep(1);
		len = sizeof(buf4);
		write(fd[1], &len, sizeof(len));
		write(fd[1], buf4, sizeof(buf4));
		sleep(1);
		close(fd[1]);
		waitpid(Beth, NULL, 0);
	}
	return 0;
}
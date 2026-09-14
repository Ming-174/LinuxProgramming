#include<stdio.h>
#define BUF_SIZE 4096
int main() {
	FILE* src = fopen("/usr/include/gnu-versions.h", "r");
	if (src == NULL) {
		perror("Fail to open gnu-versions.h");
		return 1;
	}
	else printf("gnu-versions.h is opened successfully!\n");

	FILE* dst = fopen("gnu_cp.h", "w+");
	if (dst == NULL) {
		perror("gnu_cp.h is failed to open!");
		fclose(src);
		return 1;
	}
	else printf("cp.h is opened successfully!\n");

	char buf[BUF_SIZE] = { 0 };
	size_t n;

	while ((n = fread(buf, 1, BUF_SIZE, src)) > 0) {
		fwrite(buf, 1, n, dst);
	}

	fclose(src);
	fclose(dst);
	printf("Copy is completed!\n");
	return 0;
}
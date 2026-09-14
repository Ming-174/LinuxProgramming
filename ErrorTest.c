#include<stdio.h>
#include<string.h>
#include<errno.h>
int main() {
	FILE* fp1 = fopen("server.log", "a");
	if (fp1 == NULL)return 1;
	
	setvbuf(fp1, NULL, _IOLBF, 0);
	FILE* fp2 = fopen("test", "r");
	if (fp2 == NULL) {
		fprintf(fp1,"Server ERROR! ERROR NUMBER %d,ERROR:%s\n", errno, strerror(errno));
	}
	else fclose(fp2);
	fclose(fp1);
	return 0;
}
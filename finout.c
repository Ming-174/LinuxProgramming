#include<stdio.h>
#include<sys/stat.h>
struct Buddy {
	char name[20];
	int tel;
	char address[50];
};
int main() {
	FILE* fp1 = fopen("buddy.txt", "w+");
	if (fp1 == NULL) {
		perror("Fail to create file");
		return 1;
	}
	chmod("buddy.txt", 0644);
	fprintf(fp1, "Name:%s Tel:%d address:%s", "ming", 10086, "DongRuanXueYuan");
	fclose(fp1);

	struct Buddy buddy;
	FILE* fp2 = fopen("buddy.txt", "r+");
	if (fp2 == NULL) {
		perror("Fail to open file");
		return 1;
	}
	if (fscanf(fp2,"Name:%19s Tel:%d address:%49s",buddy.name,&buddy.tel,buddy.address) == 3) {
		printf("Name:%s Tel:%d address:%s\n",buddy.name,buddy.tel,buddy.address);
	}
	else {
		perror("ERROR");
	}
	fclose(fp2);
	return 0;
}
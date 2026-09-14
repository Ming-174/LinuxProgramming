#include<stdio.h>
struct Buddy {
	char name[20];
	int tel;
	char address[50];
};
int main() {
	FILE* fp = fopen("Lt.txt", "w+");
	if (fp == NULL) {
		perror("Fail to create file");
		return 1;
	}

	struct Buddy list[3] = {
		{"Kiriko",1000,"Jp"},
		{"D.va",1001,"Kr"},
		{"Juno",1002,"Mark"}
	};

	fpos_t pos[3];
	for (int i = 0; i < 3; i++) {
		fgetpos(fp,&pos[i]);
		fprintf(fp, "Name:%s Tel:%d Address:%s\n", list[i].name, list[i].tel, list[i].address);
	}
	fflush(fp);
	printf("Three infomation has been recoded,current file size is:%ld", ftell(fp));

	struct Buddy tmp;
	fsetpos(fp, &pos[1]);
	fscanf(fp, "Name:%19s Tel:%d Address:%49s\n", tmp.name, &tmp.tel, tmp.address);
	printf("The second information,Name:%s Tel:%d Address:%s\n", tmp.name, tmp.tel, tmp.address);

	printf("Now use fread to read\n");
	fsetpos(fp, &pos[2]);
	fseek(fp, 5, SEEK_CUR);
	char buf[20] = { 0 };
	fread(buf, 4, 1, fp);
	printf("My favorite girl is %s\n", buf);

	fsetpos(fp, &pos[0]);
	fseek(fp, 5, SEEK_CUR);
	char buf1[20] = { 0 };
	fread(buf1, 6, 1, fp);
	printf("But %s is also prety hot\n", buf1);

	fclose(fp);
	return 0;
}
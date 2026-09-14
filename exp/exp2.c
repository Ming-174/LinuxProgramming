#include<stdio.h>
#include<unistd.h>
int main() {
	printf("Before first exec\n");
	int ex1 = execl("/Nonexit", "xx", NULL);
	if (ex1 == -1)printf("First exec is fail!\n");

	printf("Before second exec\n");
	char* ad[] = { "ls","-l",NULL };
	int ex2 = execvp("ls", ad);
	if (ex2 == -1)printf("Second exec is fail!\n");

	printf("After exec\n");
	return 0;
}
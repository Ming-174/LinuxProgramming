#include<stdio.h>
#include<sys/shm.h>
#include<sys/ipc.h>
#include<sys/types.h>
#include<string.h>
int main() {
	printf("Hi Beth,honey\n");
	key_t key = ftok("/home/ming174/code707/ipc", 'S');
	int shmid = shmget(key, 1024, IPC_CREAT | 0666);
	char* addr = shmat(shmid,NULL,0);
	strcpy(addr, "Hi,i love you");
	shmdt(addr);
	return 0;
}
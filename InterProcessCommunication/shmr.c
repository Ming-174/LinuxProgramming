#include<stdio.h>
#include<sys/shm.h>
#include<sys/ipc.h>
int main() {
	key_t key = ftok("/home/ming174/code707/ipc", 'S');
	int shmid = shmget(key, 1024, 0666);
	char* addr = shmat(shmid, NULL, 0);
	printf("%s\n", addr);
	shmdt(addr);
	return 0;
}
#include<stdio.h>
#include<string.h>
#include<sys/shm.h>
#include<sys/ipc.h>
#include<unistd.h>
#include<fcntl.h>
int main() {
	key_t key = ftok("/home/ming174/code707/ipc", 'S');
	int shmid = shmget(key, 1024, 0666);
	char* addr = shmat(shmid,NULL,0);
	int len = strlen(addr);
	strcpy(addr + len, "forever");
	shmdt(addr);
	sleep(20);
	shmctl(shmid, IPC_RMID, NULL);
	return 0;
}
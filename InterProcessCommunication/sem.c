#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<sys/wait.h>
#include<semaphore.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/shm.h>
#include<sys/ipc.h>
int main() {
	key_t key = ftok("/home/ming174/code707/ipc", 'R');
	int shmid = shmget(key, 1024, IPC_CREAT| 0666);
	char* addr = shmat(shmid, NULL, 0);
	addr[0] = '\0';

	sem_unlink("/sema");
	sem_t* sem = sem_open("/sema", O_CREAT | O_EXCL, 0666, 1);

	pid_t pid[2];
	char* buf[] = {
		"I love you Beth\n",
		"Yes!,Beth love you too!\n"
	};
	for (int i = 0; i < 2; i++) {
		pid[i] = fork();
		if (pid[i] == 0) {
			sem_wait(sem);
			int len = strlen(addr);
			strcpy(addr + len, buf[i]);
			sem_post(sem);
			shmdt(addr);
			_exit(0);
		}
	}

	for (int i = 0; i < 2; i++) {
		waitpid(pid[i], NULL, 0);
	}
	printf("%s", addr);
	shmdt(addr);
	sem_close(sem);
	sem_unlink("/sema");
	shmctl(shmid, IPC_RMID, NULL);
	return 0;
}
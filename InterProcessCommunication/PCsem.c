#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/wait.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<semaphore.h>
#include<string.h>
#include<sys/stat.h>
int main() {
	key_t key = ftok("/home/ming174/code707/ipc", 'R');
	if (key < 0) { perror("KEY"); return 1; }

	int shmid = shmget(key, 1024, IPC_CREAT | 0666);
	if (shmid < 0) { perror("shmget"); return 1; }

	char* addr = shmat(shmid,NULL,0);
	addr[0] = '\0';


	sem_unlink("/semC");
	sem_unlink("/semP");

	sem_t* Beth = sem_open("/semP", O_CREAT | O_EXCL, 0666, 1);
	sem_t* Harmon = sem_open("/semC", O_CREAT | O_EXCL, 0666, 0);

	pid_t pid = fork();
	if (pid == 0) {
		char* buf[] = {
			"Hi!It's so nice to see you",
			"I like you actually",
			"I LOVE YOU"
		};
		for (int i = 0; i < 3; i++) {
			sem_wait(Harmon);
			printf("%s\n", addr);
			sleep(2);
			memset(addr, 0, strlen(addr));
			strcpy(addr, buf[i]);
			sem_post(Beth);
		}
		shmdt(addr);
		shmctl(shmid, IPC_RMID, NULL);
		sem_close(Beth);
		sem_close(Harmon);
		sem_unlink("/semP");
		sem_unlink("/semC");
		_exit(0);
	}
	else {
		char* buf[] = {
			"Hi Beth!",
			"I like you my girl",
			"You're so cute"
		};
		for (int i = 0; i < 4; i++) {
			sem_wait(Beth);
			if (i == 3) {
				printf("%s\n", addr);
				sleep(2);
				sem_post(Harmon);
				break;
			}
			if (strlen(addr) != 0) {
				printf("%s\n", addr);
				sleep(2);
				memset(addr, 0, strlen(addr));
			}
			strcpy(addr,buf[i]);
			sem_post(Harmon);
		}
		waitpid(pid, NULL, 0);
		shmdt(addr);
		shmctl(shmid, IPC_RMID, NULL);
		sem_unlink("/semC");
		sem_unlink("/semP");
		sem_close(Beth);
		sem_close(Harmon);
		return 0;
	}
}
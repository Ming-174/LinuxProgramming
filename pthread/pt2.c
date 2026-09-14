#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int turn = 1;
void* ch(void* arg) {
	for (int i = 0; i < 10; i++) {
		pthread_mutex_lock(&mutex);
		while (turn != 1) {
			pthread_cond_wait(&cond, &mutex);
		}
		char buf[] = "\nI Love You, Beth\n";
		write(STDOUT_FILENO, buf, sizeof(buf) - 1);
		turn = 0;
		pthread_cond_signal(&cond);
		pthread_mutex_unlock(&mutex);

	}
	pthread_exit("Always");
}
int main() {
	pthread_t tid;
	pthread_create(&tid, NULL, ch, NULL);
	for (int i = 0; i < 10; i++) {
		pthread_mutex_lock(&mutex);
		while (turn != 0) {
			pthread_cond_wait(&cond, &mutex);
		}
		char buf[] = "\nBeth Love you\n";
		write(STDOUT_FILENO, buf, sizeof(buf) - 1);

		turn = 1;
		pthread_cond_signal(&cond);
		pthread_mutex_unlock(&mutex);
	}
	void* res;
	pthread_join(tid, &res);
	printf("%s",(char*)res);
	return 0;
}
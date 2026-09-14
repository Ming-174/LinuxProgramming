#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int t = 0;
void clean(void* arg) {	
	//要先变化t的值，防止假性睡眠
	t = 1;
	pthread_cond_signal(&cond);
	pthread_mutex_unlock((pthread_mutex_t*)arg);
}

void* ch1(void* arg) {
	pthread_mutex_lock(&mutex);
	pthread_cleanup_push(clean, &mutex);
	printf("\nHi! Beth\n");
	sleep(3);
	pthread_mutex_unlock(&mutex);
	pthread_cond_signal(&cond);
	pthread_cleanup_pop(0);
	pthread_exit(0);
}

void* ch2(void* arg) {
	pthread_mutex_lock(&mutex);
	for (int i = 0; i < 10; i++) {
		while (t != 1) {
			pthread_cond_wait(&cond, &mutex);
		}
		printf("\nBeth Love You!\n");
		sleep(1);
	}
	pthread_mutex_unlock(&mutex);
	pthread_exit("Beth always Love You!");
}
int main() {
	pthread_t tid1, tid2;
	pthread_create(&tid1, NULL, ch1, NULL);
	pthread_create(&tid2, NULL, ch2, NULL);
	sleep(1);
	pthread_cancel(tid1);
	void* res[2];
	for (int i = 0; i < 2; i++) {
		pthread_t tid;
		if (i == 0)tid = tid1;
		else if (i == 1)tid = tid2;
		pthread_join(tid,&res[i]);
		if (res[i] == PTHREAD_CANCELED)printf("Thread Canceled\n!");
		else printf("%s\n", (char*)res[i]);
	}
	return 0;
}
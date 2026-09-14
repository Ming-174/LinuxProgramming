#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
void* ch(void* arg) {
	for (long long i = 0; i < 100000000;i++) {
		write(STDOUT_FILENO, "\nBeth Love You !\n", sizeof("\nBeth Love You !\n") - 1);
		pthread_testcancel();
		write(STDOUT_FILENO, "\nBeth Love You !\n", sizeof("\nBeth Love You !\n") - 1);
	}
	pthread_exit("Always");
}
int main() {
	pthread_t tid;
	pthread_create(&tid, NULL, ch, NULL);
	write(STDOUT_FILENO, "\nI Love You, Beth\n", sizeof("\nI Love You, Beth\n") - 1);
	pthread_cancel(tid);
	void* res;
	pthread_join(tid, &res);
	if (res == PTHREAD_CANCELED)printf("Pthread canceled\n");
	return 0;
}
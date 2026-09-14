#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
void* ch(void* arg) {
	for (int i = 0; i < 5; i++) {
		char buf[] = "\nBeth Love You\n";
		write(STDOUT_FILENO, buf, sizeof(buf));
		usleep(100000);
	}
}
int main() {
	pthread_t tid;
	pthread_create(&tid, NULL, ch, NULL);
	char buf[] = "\nI Love You, Beth\n";
	for (int i = 0; i < 5; i++) {
		write(STDOUT_FILENO, buf, sizeof(buf));
		usleep(100000);
	}
	pthread_join(tid, NULL);
	return 0;
}
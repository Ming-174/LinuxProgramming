#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<signal.h>
volatile sig_atomic_t count = 0;
void handler(int sig) {
	if (count % 2 == 0) write(STDOUT_FILENO, "\nBeth Love You\n", 15);
	else write(STDOUT_FILENO, "\nI Love You,Beth\n", 17);
	count++;
}
int main() {


	struct sigaction sa;
	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGALRM, &sa, NULL);


	while (count < 6) {
		alarm(3);
		pause();
	}
	return 0;
}

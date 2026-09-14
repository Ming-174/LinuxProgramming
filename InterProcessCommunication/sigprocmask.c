#include<stdio.h>
#include<unistd.h>
#include<signal.h>
#include<string.h>
volatile sig_atomic_t count = 0;
void handler(int sig) {
	if (count % 2 == 0)write(STDOUT_FILENO, "\nI Love You,Beth\n", 17);
	else write(STDOUT_FILENO, "\nBeth Love You\n", 15);
	count++;
}
int main() {
	struct sigaction sa;
	memset(&sa, 0, sizeof(sa));
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sa.sa_handler = handler;
	sigaction(SIGALRM, &sa, NULL);

	sigset_t block, old;
	sigemptyset(&block);
	sigaddset(&block, SIGINT);
	//sigprocmask(SIG_BLOCK, &block, &old);

	sigset_t wait = old;
	while (count < 6) {
		alarm(3);
		sigsuspend(&block);
	}
	sigprocmask(SIG_UNBLOCK, &block, NULL);
	printf("Maybe you can't see this\n");
	return 0;
}
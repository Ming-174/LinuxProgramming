#include<stdio.h>
#include<signal.h>
#include<fcntl.h>
#include<unistd.h>
void handler(int sig) {
	write(STDOUT_FILENO, "Catch signal\n",13);
}
int main() {
	signal(SIGINT, handler);
	pause();
	pause();
	pause();
	return 0;
}
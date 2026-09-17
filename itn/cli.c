#include<arpa/inet.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<unistd.h>
#include<stdio.h>
#include<string.h>
#include<stdint.h>
void send_package(int fd, char* buf) {
	uint32_t net_len = htonl(strlen(buf) + 1);
	send(fd, &net_len, sizeof(net_len),0);
	send(fd, buf, strlen(buf) + 1,0);
}
int main() {
	int client_fd = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8888);
	inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
	connect(client_fd, (struct sockaddr*)&addr, sizeof(addr));
	char* buf[] = {
		"Beth",
		"Harmon",
		"Always",
		"Love",
		"You!"
	};
	for (int i = 0; i < 5; i++) {
		send_package(client_fd, buf[i]);
		printf("%s\n", buf[i]);
	}
	close(client_fd);
	return 0;
}
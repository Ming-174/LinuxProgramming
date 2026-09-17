#include<stdio.h>
#include<string.h>
#include<fcntl.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<stdint.h>
void send_package(int fd, const void* pak) {
	char* buf = (char*)pak;
	uint32_t len = htonl(strlen(buf) + 1);
	send(fd, &len, sizeof(len),0);
	send(fd, buf, strlen(buf)+1, 0);
}
int main() {
	int client_fd = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in addr;
	addr.sin_port = htons(8888);
	addr.sin_family = AF_INET;
	inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

	connect(client_fd, (struct sockaddr*)&addr, sizeof(addr));
	char* buf[] = {
		"Beth",
		"Harmon",
		"Love",
		"You"
	};
	for (int i = 0; i < 4; i++) {
		//send(client_fd, buf[i], strlen(buf[i]) + 1, 0);
		//sleep(1);
		send_package(client_fd, buf[i]);
	}
	close(client_fd);
	return 0;
}
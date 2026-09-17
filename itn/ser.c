#include<arpa/inet.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<unistd.h>
#include<stdio.h>
#include<string.h>
#include<stdint.h>
int read_exact(int fd, char* buf, size_t size) {
	ssize_t got = 0;
	while (got < size) {
		ssize_t n = recv(fd, buf + got, size - got, 0);
		if (n > 0)got += n;
		else if (n == 0)return 0;
		else return -1;
	}
	return (int)got;
}

int recv_package(int fd, char* buf,int max_len) {
	uint32_t net_len;
	int r = read_exact(fd, (char*)&net_len, sizeof(net_len));
	if (r <= 0)return r;

	uint32_t len = ntohl(net_len);
	if (len >= max_len) {
		printf("Package size exceeds buffer capacity");
		return -2;
	}
	r = read_exact(fd, buf, len);
	if (r <= 0)return r;

	return (int)len;
}

int main() {
	int server_fd = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8888);
	addr.sin_addr.s_addr = INADDR_ANY;
	bind(server_fd, (struct sockaddr*)&addr, sizeof(addr));
	listen(server_fd, 5);
	while (1) {
		int client_fd = accept(server_fd, NULL, NULL);
		char buf[1024];
		while (1) {
			int n = recv_package(client_fd, buf,sizeof(buf));
			if (n == 0) {
				printf("Client closed");
				break;
			}
			else if (n == -1) {
				perror("read_exact ERROR");
				break;
			}
			else if (n == -2) {
				break;
			}
			printf("%s\n", buf);
			memset(buf, 0, sizeof(buf));
		}
		close(client_fd);
	}
	close(server_fd);
	return 0;
}
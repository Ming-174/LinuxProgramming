#include<stdio.h>
#include<sys/socket.h>
#include<sys/select.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<errno.h>
#include<unistd.h>
#include<string.h>
int main() {
	int server_fd = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8888);
	addr.sin_addr.s_addr = INADDR_ANY;
	bind(server_fd, (struct sockaddr*)&addr, sizeof(addr));
	listen(server_fd, 128);
	int clients[128];
	for (int i = 0; i < 128; i++)clients[i] = -1;
	while (1) {
		fd_set readfds;
		FD_ZERO(&readfds);
		FD_SET(server_fd, &readfds);
		int max = server_fd;

		for (int i = 0; i < 128; i++) {
			if (clients[i] != -1) {
				FD_SET(clients[i], &readfds);
			}
			if (clients[i] > max)max = clients[i];
		}

		int rd = select(max + 1, &readfds, NULL, NULL, NULL);
		if (rd <= 0)continue;

		int add = -1;
		if (FD_ISSET(server_fd, &readfds)) {
			int client_fd = accept(server_fd, NULL, NULL);
			for (int i = 0; i < 128; i++) {
				if (clients[i] == -1) {
					clients[i] = client_fd;
					printf("Client%d is connect\n", client_fd);
					add = 1;
					break;

				}
			}
			if (add == -1) {
				printf("Clients is full\n");
				close(client_fd);
				rd--;
			}
		}

		char buf[1024];
		for (int i = 0; i < 128; i++) {
			if(clients[i]!=-1) {
				if (FD_ISSET(clients[i], &readfds)) {
					int len = recv(clients[i], buf, sizeof(buf), 0);
					if (len <= 0) {
						close(clients[i]);
						printf("%d is close\n", clients[i]);
						clients[i] = -1;
						continue;
					}
					if (len >= 1024) {
						printf("Package is over size\n");
						continue;
					}
					buf[len] = '\0';
					printf("%s\n", buf);
					memset(buf, 0, sizeof(buf));
					rd--;
				}
			}
		}
	}
}
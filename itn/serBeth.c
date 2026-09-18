#include<stdio.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<unistd.h>
#include<string.h>
#include<stdint.h>
#include<pthread.h>
#include<signal.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int clients[100];
int Client_count = 0;
char name[100][32];

int add_client(int fd) {
	pthread_mutex_lock(&mutex);
	for (int i = 0; i < 100; i++) {
		if (clients[i] == -1) {
			clients[i] = fd;
			Client_count++;
			printf("Client %d online,Total clients:%d\n", fd, Client_count);
			pthread_mutex_unlock(&mutex);
			return i;
		}
	}
	pthread_mutex_unlock(&mutex);
	printf("Clients amount is full\n");
	close(fd);
	return -1;
}

void remove_client(int fd) {
	pthread_mutex_lock(&mutex);
	for (int i = 0; i < 100; i++) {
		if (clients[i] == fd) {
			clients[i] = -1;
			name[i][0] = '\0';
			Client_count--;
			printf("Client %d is removed\n", fd);
			pthread_mutex_unlock(&mutex);
			close(fd);
			return;
		}
	}
	printf("Can't find %d", fd);
	pthread_mutex_unlock(&mutex);
}

//类型char*可以按照一字节的大小推进，使得循环读取可以按照1字节的单位进行
int read_exact(int fd, char* buf, size_t size) {
	ssize_t got = 0;
	while (got < size) {
		//从fd连接里读取数据到buf这个地址，地址的空间大小是size
		ssize_t n = recv(fd, buf + got, size - got, 0);
		if (n > 0)got += n;
		else if (n == 0)return 0;
		else return -1;
	}
	return (int)got;
}

int write_exact(int fd, char* buf, size_t size) {
	ssize_t ret = 0;
	while (ret < size) {
		ssize_t n = send(fd, buf + ret, size - ret,0);
		if (n <= 0)return -1;
		else if (n > 0)ret += n;
	}
	return ret;
}

int recv_package(int fd, char* buf, int max_len) {
	uint32_t net_len;
	int r = read_exact(fd, (char*)&net_len, sizeof(net_len));
	if (r <= 0)return r;

	uint32_t len = ntohl(net_len);
	if (len >= max_len)return -2;

	r = read_exact(fd, buf, len);
	if (r <= 0)return r;

	return r;
}

int send_package(int fd, char* buf) {
	uint32_t len = htonl(strlen(buf));
	int r = write_exact(fd, (char*)&len, sizeof(len));
	if (r < 0)return -1;

	r = write_exact(fd, buf, strlen(buf));
	if (r < 0)return -1;
	return r;
}

void broadcast(int fd, char* buf) {
	int tmp[100];
	char tmpname[32]="Unknown";
	int idx = 0;
	int client_idx = -1;
	pthread_mutex_lock(&mutex);
	for (int i = 0; i < 100; i++) {
		if (clients[i] != -1 && clients[i] != fd) {
			tmp[idx++] = clients[i];
		}
		else if (clients[i] == fd) {
			snprintf(tmpname, sizeof(tmpname), "%s", name[i]);
		}
	}
	pthread_mutex_unlock(&mutex);
	char out[1100];
	snprintf(out, sizeof(out), "%s:%s", tmpname, buf);
	printf("%s\n", out);
	for (int i = 0; i < idx; i++) {
		int s = send_package(tmp[i], out);
		if (s < 0) {
			perror("Disconnect");
		}
	}
}

void* handler(void* arg) {
	int client_fd = (int)(intptr_t)arg;
	int idx = add_client(client_fd);
	if (idx == -1) {
		pthread_exit(NULL);
	}

	char nm[32];
	int name_len = recv_package(client_fd, nm,sizeof(nm));
	if (name_len == 0) {
		printf("Client closed\n");
		remove_client(client_fd);
		pthread_exit(NULL);
	}
	if (name_len == -1) {
		perror("read_exact ERROR");
		remove_client(client_fd);
		pthread_exit(NULL);
	}
	if (name_len == -2) {
		printf("package is over size\n");
		remove_client(client_fd);
		pthread_exit(NULL);
	}
	nm[name_len] = '\0';
	pthread_mutex_lock(&mutex);
	snprintf(name[idx], sizeof(name[idx]), "%s", nm);
	pthread_mutex_unlock(&mutex);

	char buf[1024] = { 0 };
	//char confession[] = "Beth Love You";
	while(1){
		int len = recv_package(client_fd, buf, sizeof(buf));
		//主要退出窗口:当客户端端口关闭，recv返回0->read_exact返回0->recv_package返回0
		if (len == 0) {
			printf("Client closed\n");
			break;
		}
		if (len == -1) {
			perror("read_exact ERROR");
			break;
		}
		if (len == -2) {
			printf("package is over size\n");
			break;
		}
		buf[len] = '\0';
		//printf("%s\n",buf);
		//服务器中，server_fd是只能等待数据的接口，client_fd是发送的端口
		//send_package(client_fd, confession);
		broadcast(client_fd, buf);
	}
	remove_client(client_fd);
}


int main() {
	signal(SIGPIPE, SIG_IGN);
	for (int i = 0; i < 100; i++)clients[i] = -1;
	int server_fd = socket(AF_INET, SOCK_STREAM, 0);

	//当客户端连接后，服务器关闭后再重启，会重启失败，因为端口还处于TIME_WAIT状态
	//这样的设置可以让服务器快速重启
	int opt=1;
	setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8888);
	addr.sin_addr.s_addr = INADDR_ANY;
	if ((bind(server_fd, (struct sockaddr*)&addr, sizeof(addr))) < 0) {
		perror("bind");
		return 1;
	}
	if ((listen(server_fd, 5))<0) {
		perror("listen");
		return 1;
	}
	while (1) {
		int client_fd = accept(server_fd, NULL, NULL);

		pthread_t tid;
		//该接口的第四个参数是void*类型，但是会出现一个数据竞争问题
		//当handler还没来得及进行转型和赋值时，第二个客户端接入
		//而第二个客户端会修改client_fd地址的值，而前一个客户端读取client_fd的地址就会使用错误的fd
		//这里使用intptr，他是一个安全的将int数据直接转成指针的中间人
		//这样可以直接通过一个传指针的接口，把数据真的做成一个指针传送过去
		pthread_create(&tid, NULL, &handler, (void*)(intptr_t) client_fd);
		pthread_detach(tid);
	}
}
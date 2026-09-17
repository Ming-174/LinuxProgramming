#include<stdio.h>
#include<unistd.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<string.h>
#include<stdint.h>
//精确读取一个数据，传入数据的地址，数据的长度
//我们要求解析一个类型的数据，字节数是固定的
//转char*核心目的是为了可以在循环读取的时候使用buf + got这样的结构进行固定字节的推进
// 因为char类型就是单个字节的长度，而其他类型就不一定
// uint32类型如果+1就变成四字节推进，因为uint32就是四字节的
//因为这样的地址推进+1的步长，是按照变量类型的长度+1的
int read_exact(int fd, char* buf, size_t size) {
	ssize_t got = 0;
	//recv(fd,buf,size,0)
	while (got < size) {
		ssize_t n = recv(fd, buf + got, size - got, 0);
		if (n > 0)got += n;
		else if (n == 0)return n;
		else return -1;
	}
	return 1;
}
int recv_package(int fd,char*buf) {
	uint32_t net_len;
	ssize_t r = read_exact(fd, (char*)&net_len, sizeof(net_len));
	if (r <= 0)return r;

	uint32_t len = ntohl(net_len);
	r = read_exact(fd, buf, len);
	if (r <= 0)return r;

	return len;
}
int main() {
	//建立服务器通道
	int server_fd = socket(AF_INET, SOCK_STREAM, 0);
	//建立地址
	struct sockaddr_in addr;
	//设置端口8888
	addr.sin_port = htons(8888);
	//设置地址族为IPV4
	addr.sin_family = AF_INET;
	//设置接受来源为所有网口
	addr.sin_addr.s_addr = INADDR_ANY;
	//绑定服务器地址与服务器通道
	bind(server_fd, (struct sockaddr*)&addr, sizeof(addr));
	//开启监听模式
	listen(server_fd, 5);
	while (1) {
		int client_fd = accept(server_fd, NULL, NULL);
		printf("Client connected!\n");

		char buf[1024] = { 0 };
		//ssize_t n;
		//while ((n = recv(client_fd, buf, sizeof(buf), 0)) > 0) {
		//	printf("%s\n", buf);
		//	memset(buf, 0, sizeof(buf));
		//}
		while (1) {
			ssize_t n = recv_package(client_fd, buf);
			if (n == 0) {
				printf("Client close\n");
				break;
			}
			if (n < 0) {
				perror("read_exact");
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
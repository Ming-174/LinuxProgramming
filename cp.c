#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
int main(){
int fd1 = open("a.txt",O_RDONLY);
if( fd1 < 0 )return -1;
int fd2 = open("b.txt",O_WRONLY | O_CREAT | O_TRUNC , 0644 );
if( fd2 < 0 ){
	close(fd1);
	return -1;
	}
int n;
char buf[1024];
while( ( n = read(fd1,buf,sizeof(buf)) ) > 0 ){
	int written = 0;
	while(written<n){
		int ret = write(fd2,buf+written,n-written);
		if(ret<0){
			close(fd1);
			close(fd2);
			return -1;
			}
		written+=ret;
		}
	}
close(fd1);
close(fd2);
return 0;
}

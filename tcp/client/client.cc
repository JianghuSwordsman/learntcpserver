#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main()
{
    int fd = socket(AF_INET,SOCK_STREAM,0);
    if(-1 == fd)
    {
        perror("socket error");
        exit(EXIT_FAILURE);
    }
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8888);
    addr.sin_addr.s_addr = inet_addr("192.168.31.82");

    int ret = connect(fd,(struct sockaddr *)&addr,sizeof(struct sockaddr_in));
    if(-1 == ret)
    {
        perror("connect error");
        exit(EXIT_FAILURE);
    }
    printf("connect success\n");
    char buf[1024] = {0};
    ret = read(fd,buf,sizeof(buf));
    printf("%s",buf);

    while(1)
    {
        memset(buf, 0, sizeof(buf));
        printf("please input:\n");
        fgets(buf,sizeof(buf),stdin);
        write(fd,buf,strlen(buf));
        read(fd,buf,sizeof(buf));
        printf("%s");
    }
}

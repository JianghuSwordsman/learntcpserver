#include "SocketIO.h"
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>

ssize_t SocketIO::readn(char *buf, size_t count)
{
    size_t nleft = count;//剩余的字节数
    ssize_t nread;//返回值，每次read调用返回，即读取的字节数
    char *bufp = buf;//缓冲区的偏移量

    while(nleft > 0)
    {
        nread = ::read(sockfd_, bufp, nleft);
        if(nread == -1)
        {
            if(errno == EINTR)
                continue;
            return -1;
        }
        else if(nread == 0)
            break;

        nleft -= nread;
        bufp += nread;
    }

    return (count - nleft);
}

ssize_t SocketIO::writen(const char *buf, size_t count)
{
    size_t nleft = count;
    ssize_t nwrite;
    const char *bufp = buf;

    while(nleft > 0)
    {
        nwrite = ::write(sockfd_, bufp, nleft);        
        if(nwrite <= 0)            
        {
            if(nwrite == -1 && errno == EINTR)
                continue;
            return -1;
        }
        nleft -= nwrite;
        bufp += nwrite;
    }

    return count;
}

ssize_t SocketIO::recv_peek(char *buf, size_t len)
{
    int nread;
    do
    {
        nread = ::recv(sockfd_, buf, len, MSG_PEEK);
    }
    while(nread == -1 && errno == EINTR);

     return nread;
}

ssize_t SocketIO::readline(char *usrbuf, size_t maxlen)
{
    size_t nleft = maxlen -1;//最大可读取的长度为缓冲区长度-1
    char *bufp = usrbuf;//缓冲区位置
    size_t total = 0;//读取的字节数

    ssize_t nread;
    while(nleft > 0)
    {
        nread = recv_peek(bufp, nleft);
        if(nread <= 0)
            return nread;

        int i;
        for(i = 0; i < nread; ++i)
        {
            if(bufp[i] == '\n')
            {
                size_t nsize = i+1;
                if(readn(bufp, nsize) != static_cast<ssize_t>(nsize))
                    return -1;

                bufp += nsize;
                total += nsize;
                *bufp = '\0';
                return total;
            }
        }
        //没找到\n
        if(readn(bufp, nread) != nread)
            return -1;

        bufp += nread;
        total += nread;
        nleft -= nread;
    }
    *bufp = '\0';
    return maxlen - 1;
}

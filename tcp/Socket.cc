#include "Socket.h"
#include <errno.h>
#include "InetAddress.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

namespace wd
{
Socket::Socket(int sockfd) :sockfd_(sockfd){}

Socket::Socket()
{
    sockfd_ = socket(AF_INET,SOCK_STREAM,0);
    if(-1 == sockfd_)
    {
        perror("sockfd error");
    }
}

void Socket::ready(const InetAddress & addr)
{
    setReuseAddr(true);
    setReusePort(true);
    bindAddress(addr);
    listen();
}
int Socket::accept()
{
    int fd = ::accept(sockfd_,NULL,NULL);
    if(-1 == fd)
    {
        perror("accpet error");
        ::close(sockfd_);
    }
    return fd;
}
void Socket::shutdownWrite()
{
    if(-1 == ::shutdown(sockfd_,SHUT_WR))
    {
        perror("shutdown error");
        exit(EXIT_FAILURE);
    }
}
void Socket::bindAddress(const InetAddress & addr)
{
    if(-1 == ::bind(sockfd_,(const struct sockaddr*)addr.getSockAddrPtr(),sizeof(InetAddress)))
    {
        perror("bind error");
        ::close(sockfd_);
        exit(EXIT_FAILURE);
    }
}
void Socket::listen()
{
    if(-1 == ::listen(sockfd_,10))
    {
        perror("listen error");
        ::close(sockfd_);
        exit(EXIT_FAILURE);
    }
}

void Socket::setReuseAddr(bool flag)
{
    int on = (flag ? 1:0);
    if(-1 == ::setsockopt(sockfd_,SOL_SOCKET,SO_REUSEADDR,&on,static_cast<socklen_t>(sizeof(on))))
    {
      perror("setsockopt reuseaddr error");
      ::close(sockfd_);
      exit(EXIT_FAILURE);
    }
}
void Socket::setReusePort(bool flag)
{
#ifdef SO_REUSEPORT
    int on = (flag ? 1:0);
    if(-1 == ::setsockopt(sockfd_,SOL_SOCKET,SO_REUSEPORT,&on,static_cast<socklen_t>(sizeof(on))))
    {
      perror("setsockopt reuseport error");
      ::close(sockfd_);
      exit(EXIT_FAILURE);
    }
#else
    if(flag)
    {
       fprintf(stderr,"SO_REUSEPORT is not supported!\n");
    }
#endif
}
InetAddress Socket::getLocalAddr(int sockfd)
{
    struct sockaddr_in addr;
    socklen_t len = sizeof(sockaddr_in);
    if(-1 == ::getsockname(sockfd,(struct sockaddr *)&addr,&len))
    {
        perror("getsockname error");
    }
    return InetAddress(addr);
}
InetAddress Socket::getPeerAddr(int sockfd)
{
    struct sockaddr_in addr;
    socklen_t len = sizeof(sockaddr_in);
    if(-1 == ::getpeername(sockfd,(struct sockaddr *)&addr,&len))
    {
        perror("getpeername error");
    }
    return InetAddress(addr);
}
}//end of namespace wd

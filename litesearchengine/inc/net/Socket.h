#ifndef _SOCKET_H_
#define _SOCKET_H_

#include "Noncopyable.h"
#include "InetAddress.h"

class Socket : Noncopyable
{
public:
    explicit Socket(int sockfd);
    ~Socket();

    int fd() const { return sockfd_; }
    void bindAddress(const InetAddress &addr);
    void listen();
    int accept();

    void shutdownWrite();
    //nagle算法的作用为，限制网络中只能出现一个小于MSS的数据包，可提高网络利用率，
    //但是增加了延时，为了降低延时，on为true则禁用nagle算法
    void setTcpNoDelay(bool on);
    void setReuseAddr(bool on);
    void setReusePort(bool on);
    void setKeepAlive(bool on);

    static InetAddress getLocalAddr(int sockfd);
    static InetAddress getPeerAddr(int sockfd);
private:
    const int sockfd_;
};
#endif

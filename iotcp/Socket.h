#ifndef WD_SOCKET_H_
#define WD_SOCKET_H_

#include "Noncopyable.h"

namespace wd
{
class InetAddress;

class Socket : Noncopyable
{
public:
    Socket(int sockfd);
    Socket();

    void ready(const InetAddress & addr);
    int accept();
    void shutdownWrite();

    static InetAddress getLocalAddr(int sockfd);
    static InetAddress getPeerAddr(int sockfd);
private:
    void bindAddress(const InetAddress & addr);
    void listen();
    void setReuseAddr(bool flag);
    void setReusePort(bool flag);
private:
    int sockfd_;

};
}//end of namespace wd
#endif

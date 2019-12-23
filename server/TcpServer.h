#ifndef WD_TCPSERVER_H_
#define WD_TCPSERVER_H_

#include "Socket.h"
#include "EpollPoller.h"

namespace wd
{

class TcpServer
{
public:
    typedef TcpConnection::TcpConnectionPtrCallback TcpServerCallback;
    TcpServer(unsigned short port);
    TcpServer(const char * pIp,unsigned short port);

    void start();
    void stop();
    
    void setConnectionCallback(TcpServerCallback cb);
    void setMessageCallback(TcpServerCallback cb);
    void setCloseCallback(TcpServerCallback cb);
private:
    InetAddress inetAddr_;
    Socket sockfd_;
    EpollPoller  epollfd_;
};
}//end of namespace wd
#endif

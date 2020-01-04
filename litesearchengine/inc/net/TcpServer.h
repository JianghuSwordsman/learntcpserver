#ifndef _TCPSERVER_H_
#define _TCPSERVER_H_

#include "Noncopyable.h"
#include "EpollPoller.h"
#include "Socket.h"

class TcpServer : Noncopyable
{
public:
    typedef EpollPoller::EpollCallback TcpServerCallback;
    explicit TcpServer(const InetAddress &addr);

    void start()
    { poller_.loop(); }

    void setConnectCallback(TcpServerCallback cb)
    { poller_.setConnectCallback(std::move(cb)); }

    void setMessageCallback(TcpServerCallback cb)
    { poller_.setMessageCallback(std::move(cb)); }

    void setCloseCallback(TcpServerCallback cb)
    { poller_.setCloseCallback(std::move(cb)); }
private:
    Socket sockfd_;
    EpollPoller poller_;
};
#endif

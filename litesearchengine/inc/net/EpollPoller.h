#ifndef _EPOLLPOLLER_H_
#define _EPOLLPOLLER_H_

#include "Noncopyable.h"
#include "TcpConnection.h"
#include <vector>
#include <map>
#include <sys/epoll.h>

class EpollPoller :Noncopyable
{
public:
    typedef TcpConnection::TcpConnectionCallback EpollCallback;
    explicit EpollPoller(int listenfd);
    ~EpollPoller();

    //启动epoll，包含一个while循环
    void loop();
    //关闭epoll
    void unloop();

    void setConnectCallback(EpollCallback cb)
    { onConnectCallback_ = std::move(cb); }
    void setMessageCallback(EpollCallback cb)
    { onMessageCallback_ = std::move(cb); }
    void setCloseCallback(EpollCallback cb)
    {onCloseCallback_ = std::move(cb); }
private:
    //循环并epoll_wait
    void waitEpollFd();
    //accept处理
    void handleConnection();
    //处理msg，recv
    void handleMessage(int peerfd);

private:
    const int epollfd_;
    const int listenfd_;
    bool isLooping_;

    //保存活跃的fd
    typedef std::vector<struct epoll_event> EventList;
    EventList events_;

    //fd到conn的映射
    typedef std::map<int, TcpConnectionPtr> ConnectionList;
    ConnectionList lists_;
    
    //处理连接
    EpollCallback onConnectCallback_;
    //从活跃fd中recv数据
    EpollCallback onMessageCallback_;
    //关闭连接，并从epoll中delete
    EpollCallback onCloseCallback_;

};


#endif

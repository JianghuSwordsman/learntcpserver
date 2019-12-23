#ifndef WD_EPOLLPOLLER_H_
#define WD_EPOLLPOLLER_H_

#include "Noncopyable.h"
#include "TcpConnection.h"
#include <vector>
#include <sys/epoll.h>
#include <map>
namespace wd
{

class EpollPoller : Noncopyable
{
public:
    typedef TcpConnection::TcpConnectionPtrCallback EpollCallback;
    EpollPoller(int listenfd);
    ~EpollPoller();

    void loop();
    void unloop();
    void setConnectionCallback(EpollCallback cb);
    void setMessageCallback(EpollCallback cb);
    void setCloseCallback(EpollCallback cb);

private:
    void waitEpollfd();
    void handleConnection();
    void handleMessage(int peerfd);
private:
    int epollfd_;
    int listenfd_;
    bool isLooping_;

    typedef std::vector<struct epoll_event> EventList;
    EventList eventsList_;

    typedef std::map<int, TcpConnectionPtr> ConnectionMap;
    ConnectionMap connMap_;

    EpollCallback onConnectionCb_;
    EpollCallback onMessageCb_;
    EpollCallback onCloseCb_;
};
}//end of namespace wd
#endif

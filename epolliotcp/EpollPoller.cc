#include "EpollPoller.h"
#include <sys/epoll.h>
#include <errno.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

namespace wd
{
//创建epoll
int createEpollFd()     
{
    int efd = ::epoll_create1(0);
    if(-1 == efd)
    {
        perror("epoll_create1 error");
        exit(EXIT_FAILURE);
    }
}
//添加连接到epoll监听
void addEpollReadFd(int efd, int fd)
{
    struct epoll_event ev;
    ev.data.fd = fd;
    ev.events = EPOLLIN;
    int ret = epoll_ctl(efd, EPOLL_CTL_ADD, fd, &ev);
    if(-1 == ret)
    {
        perror("epoll_ctl add error");
        exit(EXIT_FAILURE);
    }
}
//删除连接到epoll监听
void delEpollReadFd(int efd, int fd)
{
    struct epoll_event ev;
    ev.data.fd = fd;
    int ret = ::epoll_ctl(efd, EPOLL_CTL_DEL, fd, &ev);
    if(-1 == ret)
    {
        perror("epoll_ctl add error");
        exit(EXIT_FAILURE);
    }
    close(fd);
}
int acceptConnFd(int listenfd)
{
    int peerfd = ::accept(listenfd, NULL, NULL);
    if(peerfd == -1)
    {
        perror("accept conn fd");
        exit(EXIT_FAILURE);
    }
    return peerfd;
}
//预览数据
ssize_t recvPeek(int sockfd, void *buf, size_t len)
{
    int nread;
    do
    {
        nread = :: recv(sockfd, buf, len, MSG_PEEK);
    }while(nread == -1 && errno == EINTR);

    return nread;
}
//通过预览数据 判断conn是否关闭
bool isConnectionClosed(int sockfd)
{
    char buf[1024] = {0};
    ssize_t nread = recvPeek(sockfd, buf, sizeof(buf));
    if(nread == -1)
    {
        perror("recvPeek");
        exit(EXIT_FAILURE);
    }
    return (nread == 0);
}
//---------------------------------------------------
EpollPoller::EpollPoller(int listenfd)
    :epollfd_(createEpollFd()),
    listenfd_(listenfd),
    isLooping_(false),
    eventsList_(1024)
{
    addEpollReadFd(epollfd_,listenfd);
}

EpollPoller::~EpollPoller()
{
    ::close(epollfd_);
}
void EpollPoller::loop()
{
    isLooping_ = true;
    while(isLooping_)
    {
        waitEpollfd();
    }
}
void EpollPoller::unloop()
{
    if(isLooping_)
        isLooping_ = false;
}
void EpollPoller::setConnectionCallback(EpollCallback cb)
{
    onConnectionCb_ = cb;
}

void EpollPoller::setMessageCallback(EpollCallback cb)
{
    onMessageCb_ = cb;
}

void EpollPoller::setCloseCallback(EpollCallback cb)
{
    onCloseCb_ = cb;
}

void EpollPoller::waitEpollfd()
{
    int nready;
    do
    {
        nready = ::epoll_wait(epollfd_, &(*eventsList_.begin()),eventsList_.size(),5000);
    }while(nready == -1 && errno == EINTR);

    if(nready == -1)
    {
        perror("epoll_wait error");
        exit(EXIT_FAILURE);
    }
    else if(nready == 0)
    {
        printf("epoll_wait timeout\n");
    }
    else
    {
        if(nready == static_cast<int>(eventsList_.size()))
        {
            eventsList_.resize(eventsList_.size() * 2);
        }

        for(int idx = 0; idx != nready; ++idx)
        {
            if(eventsList_[idx].data.fd == listenfd_)
            {
              if(eventsList_[idx].events & EPOLLIN)
              {
                  handleConnection();
              }
            }
            else
            {
                if(eventsList_[idx].events & EPOLLIN)
                {
                    handleMessage(eventsList_[idx].data.fd);
                }
            }
        }//end for
    }//end else
}

void EpollPoller::handleConnection()
{
    int peerfd = acceptConnFd(listenfd_);
    addEpollReadFd(epollfd_, peerfd);

    TcpConnectionPtr conn(new TcpConnection(peerfd));
    conn->setConnectionCallback(onConnectionCb_);
    conn->setMessageCallback(onMessageCb_);
    conn->setCloseCallback(onCloseCb_);

    std::pair<ConnectionMap::iterator,bool> ret;
    ret = connMap_.insert(std::make_pair(peerfd,conn));
    assert(ret.second == true);
    (void)ret;

    //connMap_[oeerfd] = conn;
    conn->handleConnectionCallback();
}

void EpollPoller::handleMessage(int peerfd)
{
    bool isClosed = isConnectionClosed(peerfd);
    ConnectionMap::iterator it = connMap_.find(peerfd);
    assert(it != connMap_.end());

    if(isClosed)
    {
        it->second->handleCloseCallback();
        delEpollReadFd(epollfd_, peerfd);
        connMap_.erase(it);
    }
    else
    {
        it->second->handleMessageCallback();
    }
}

}//end of namespace wd

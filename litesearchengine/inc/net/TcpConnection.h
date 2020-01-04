#ifndef _TCPCONNECTION_H_
#define _TCPCONNECTION_H_

#include "Noncopyable.h"
#include "Socket.h"
#include "SocketIO.h"
#include <memory>
#include <functional>

class TcpConnection;
typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;

class TcpConnection : Noncopyable,
    public std::enable_shared_from_this<TcpConnection>
{
public:
    typedef std::function<void(const TcpConnectionPtr &)> TcpConnectionCallback;

    explicit TcpConnection(int sockfd);
    ~TcpConnection();

    void setConnectCallback(TcpConnectionCallback cb)
    { onConnectCallback_ = std::move(cb); }
    void setMessageCallback(TcpConnectionCallback cb)
    { onMessageCallback_ = std::move(cb); }
    void setCloseCallback(TcpConnectionCallback cb)
    { onCloseCallback_ = std::move(cb); }

    void handleConnectCallback();
    void handleMessageCallback();
    void handleCloseCallback();
    //半关闭套接字写端
    void shutdown()
    { sockfd_.shutdownWrite(); isShutdownWrite_ = true; }
    //返回套接字句柄
    int fd()
    { return sockfd_.fd(); }

    ssize_t readn(char *buf, size_t count);
    ssize_t writen(const char *buf, size_t count);
    ssize_t readLine(char *usrbuf, size_t maxlen);
    std::string receive();
    void send(const std::string &s);

    const InetAddress &getLocalAddr()const
    { return localAddr_; }
    const InetAddress &getPeerAddr()const
    { return peerAddr_; }
    //把套接字双方的地址信息转化为string字符串
    std::string toString()const;
private:
    Socket sockfd_;
    SocketIO sockIO_;
    //本地地址信息
    const InetAddress localAddr_;
    //对端地址信息
    const InetAddress peerAddr_;
    //是否关闭套接字写端
    bool isShutdownWrite_;

    TcpConnectionCallback onConnectCallback_;
    TcpConnectionCallback onMessageCallback_;
    TcpConnectionCallback onCloseCallback_;
};
#endif

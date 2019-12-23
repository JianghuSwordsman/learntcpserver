#ifndef WD_TCPCONNECTION_H_
#define WD_TCPCONNECTION_H_

#include "Noncopyable.h"
#include "SocketIO.h"
#include "Socket.h"
#include "InetAddress.h"

#include <memory>
#include <functional>
#include <string>

namespace wd
{
class TcpConnection;
typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;

class TcpConnection : Noncopyable, public std::enable_shared_from_this<TcpConnection>
{
public:
    typedef std::function<void (const TcpConnectionPtr &)> TcpConnectionPtrCallback;
    TcpConnection(int sockfd);
    ~TcpConnection();
    std::string receive();
    void send(const std::string & msg);
    void shutdown();

    void setConnectionCallback(TcpConnectionPtrCallback cb);
    void handleConnectionCallback();
    void setMessageCallback(TcpConnectionPtrCallback cb);
    void handleMessageCallback();
    void setCloseCallback(TcpConnectionPtrCallback cb);
    void handleCloseCallback();

    std::string toString();
private:
    Socket sockfd_;
    SocketIO sockIO_;
    const InetAddress localAddr_;
    const InetAddress peerAddr_;
    bool isShutdownWrite_;

    TcpConnectionPtrCallback onConnectionCb_;
    TcpConnectionPtrCallback onMessageCb_;
    TcpConnectionPtrCallback onCloseCb_;
};
}//end of namesapce wd
#endif

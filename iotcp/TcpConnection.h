#ifndef WD_TCPCONNECTION_H_
#define WD_TCPCONNECTION_H_

#include "SocketIO.h"
#include "Socket.h"
#include "InetAddress.h"

namespace wd
{
class TcpConnection : Noncopyable
{
public:
    TcpConnection(int sockfd);
    ~TcpConnection();
    std::string receive();
    void send(const std::string & msg);
    void shutdown();
private:
    Socket sockfd_;
    SocketIO sockIO_;
    const InetAddress localAddr_;
    const InetAddress peerAddr_;
    bool isShutdownWrite_;
};
}//end of namesapce wd
#endif

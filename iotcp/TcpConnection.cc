#include "TcpConnection.h"

namespace wd
{
TcpConnection::TcpConnection(int sockfd)
    :sockfd_(sockfd),
    sockIO_(sockfd),
    localAddr_(wd::Socket::getLocalAddr(sockfd)),
    peerAddr_(wd::Socket::getPeerAddr(sockfd)),
    isShutdownWrite_(false)
{}
TcpConnection::~TcpConnection()
{
    if(!isShutdownWrite_)
    {
        isShutdownWrite_ = true;
    }
}
std::string TcpConnection::receive()
{
    char buf[1024] = {0};
    size_t ret = sockIO_.readline(buf, sizeof(buf));
    if(ret == 0)
    {
        return std::string();
    }
    else
    {
        return std::string(buf);
    }
}

void TcpConnection::send(const std::string & msg)
{
    sockIO_.writen(msg.c_str(),msg.size());
}

void TcpConnection::shutdown()
{
    sockfd_.shutdownWrite();
}
}//end of namesapce

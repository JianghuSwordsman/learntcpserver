#include "TcpConnection.h"
#include <string>
#include <stdio.h>
#include <iostream>
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

void TcpConnection::setConnectionCallback(TcpConnectionPtrCallback cb)
{
    onConnectionCb_ = cb;
}

void TcpConnection::handleConnectionCallback()
{
    if(onConnectionCb_)
    {
        onConnectionCb_(shared_from_this());
    }
}
void TcpConnection::setMessageCallback(TcpConnectionPtrCallback cb)
{
    onMessageCb_ = cb;
}

void TcpConnection::handleMessageCallback()
{
    if(onMessageCb_)
    {
        onMessageCb_(shared_from_this());
    }
}
void TcpConnection::setCloseCallback(TcpConnectionPtrCallback cb)
{
    onCloseCb_ = cb;
}

void TcpConnection::handleCloseCallback()
{
    if(onCloseCb_)
    {
        onCloseCb_(shared_from_this());
    }
}
std::string TcpConnection::toString()
{
	char str[100] = {0};
	snprintf(str, sizeof(str), "%s:%d -> %s:%d",
			 localAddr_.ip().c_str(),
			 localAddr_.port(),
			 peerAddr_.ip().c_str(),
			 peerAddr_.port());
	return std::string(str);
}
}//end of namesapce

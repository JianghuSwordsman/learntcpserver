#include "InetAddress.h"
#include "Socket.h"
#include "EpollPoller.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <memory>
#include <iostream>

void onConnection(const wd::TcpConnectionPtr & conn)
{
    std::cout << conn->toString() << std::endl;
    //printf("%s\n",conn->toString().c_str());
    conn->send("Hello, welcome to wd's sever\n");
}

void onMessage(const wd::TcpConnectionPtr & conn)
{
    std::string msg(conn->receive());
    conn->send(msg);
}
void onClose(const wd::TcpConnectionPtr & conn)
{
    std::cout << conn->toString().c_str() << " close" << std::endl;
}

int main()
{
    wd::InetAddress inetaddr(8888);
    wd::Socket socket;
    socket.ready(inetaddr);

    wd::EpollPoller epollfd(socket.fd());
    epollfd.setConnectionCallback(onConnection);
    epollfd.setMessageCallback(onMessage);
    epollfd.setCloseCallback(onClose);
    epollfd.loop();

    return 0;
}

#include "TcpServer.h"
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
    std::cout << "client:" << msg << std::endl;
    conn->send(msg);
}
void onClose(const wd::TcpConnectionPtr & conn)
{
    std::cout << conn->toString().c_str() << " close" << std::endl;
}

int main()
{
    wd::TcpServer server(8888);
    server.setConnectionCallback(onConnection);
    server.setMessageCallback(onMessage);
    server.setCloseCallback(onClose);
    server.start();

    return 0;
}

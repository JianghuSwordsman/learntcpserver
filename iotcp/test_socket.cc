#include "InetAddress.h"
#include "TcpConnection.h"
#include "Socket.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <memory>
#include <iostream>
int main()
{

    wd::InetAddress inetaddr(8888);

    wd::Socket socket;
    socket.ready(inetaddr);

    int peerfd = socket.accept();
    wd::InetAddress peerAddr = wd::Socket::getPeerAddr(peerfd);
    printf("%s::%d has connected!\n",peerAddr.ip().c_str(),peerAddr.port());

    std::unique_ptr<wd::TcpConnection> conn(new wd::TcpConnection(peerfd));
    conn->send("welcome to sever.");

    while(1)
    {
        std::string str = conn->receive();
        std::cout << "receive: " << str << std::endl;
        conn->send(str);
    }
}

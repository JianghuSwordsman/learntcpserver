#include "InetAddress.h"
#include "Socket.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <string>


int main()
{

    wd::InetAddress inetaddr(8888);

    wd::Socket socket;
    socket.ready(inetaddr);

    int peerfd = socket.accept();
    wd::InetAddress peerAddr = wd::Socket::getPeerAddr(peerfd);
    printf("%s::%d has connected!\n",peerAddr.ip().c_str(),peerAddr.port());

    char buf[1024] = "welcome to server.";
    ::write(peerfd,buf,strlen(buf));

    while(1)
    {
        memset(buf,0,sizeof(buf));
        int ret = ::read(peerfd,buf,1024);
        ::write(peerfd,buf,strlen(buf));
    }
}

#include "InetAddress.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>

namespace wd
{
InetAddress::InetAddress(unsigned short port)
{
    addr_.sin_family = AF_INET;
    addr_.sin_port = htons(port);
    addr_.sin_addr.s_addr = INADDR_ANY;
}

InetAddress::InetAddress(const char * pIp,unsigned short port)
{
    addr_.sin_family = AF_INET;
    addr_.sin_port = htons(port);
    addr_.sin_addr.s_addr = inet_addr(pIp);   
}
    
InetAddress::InetAddress(const struct sockaddr_in & addr)
    :addr_(addr)
{}
const struct sockaddr_in * InetAddress::getSockAddrPtr()const
{
    return &addr_;
}
std::string InetAddress::ip()
{
    return std::string(inet_ntoa(addr_.sin_addr));
}

unsigned short InetAddress::port()
{
    return ntohs(addr_.sin_port);
}

}//end of namespace wd

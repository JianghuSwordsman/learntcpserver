#ifndef WD_INETADDRESS_H_
#define WD_INETADDRESS_H_

#include <string>
#include <netinet/in.h>

namespace wd
{
class InetAddress
{
public:
    InetAddress(unsigned short port);
    InetAddress(const char* pIp,unsigned short port);
    InetAddress(const struct sockaddr_in & addr);

    const struct sockaddr_in * getSockAddrPtr()const;
    std::string ip();
    unsigned short port();
private:
    struct sockaddr_in addr_;
};
}//end of namesapce wd
#endif

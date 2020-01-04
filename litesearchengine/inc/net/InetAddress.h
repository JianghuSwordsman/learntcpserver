#ifndef _INETADDRESS_H_
#define _INETADDRESS_H_

#include <string>
#include <netinet/in.h>

typedef struct sockaddr SA;
typedef struct sockaddr_in SAI;

class InetAddress
{
public:
    explicit InetAddress(uint16_t port);
    InetAddress(const std::string &ip, uint16_t port);
    InetAddress(const struct sockaddr_in &addr);

    void setSockAddrInet(const struct sockaddr_in&addr)
    { addr_ = addr; }
    const struct sockaddr_in * getSockAddrInet() const
    { return &addr_; }

    std::string toIp() const;
    uint16_t toPort() const;
private:
    struct sockaddr_in addr_;
};
#endif

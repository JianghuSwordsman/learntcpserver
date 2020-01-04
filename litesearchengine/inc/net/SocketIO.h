#ifndef _SOCKETIO_H_
#define _SOCKETIO_H_

#include "Noncopyable.h"
#include <sys/types.h>

class SocketIO : Noncopyable
{
public:
    explicit SocketIO(int sockfd)
        :sockfd_(sockfd)
    {}
    ssize_t readn(char *buf, size_t count);
    ssize_t writen(const char *buf, size_t count);
    ssize_t readline(char *usrbuf, size_t maxlen);
private:
    //预读取
    ssize_t recv_peek(char *buf, size_t len);

    const int sockfd_;
};
#endif

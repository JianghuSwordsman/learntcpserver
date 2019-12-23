#ifndef WD_SOCKETIO_H_
#define WD_SOCKETIO_H_

#include <unistd.h>
namespace wd
{
class SocketIO
{
public:
    SocketIO(int sockfd);

    ssize_t readn(char *buf,size_t count);
    ssize_t writen(const char *buf,size_t count);
    ssize_t readline(char *buf,size_t max_len);
private:
    ssize_t recv_peek(char *buf,size_t count);
private:
    int sockfd_;
};
}//end of namespace wd
#endif

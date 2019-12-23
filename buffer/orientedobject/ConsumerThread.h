#ifndef WD_CONSUMERTHREAD_H_
#define WD_CONSUMERTHREAD_H_

#include "Thread.h"

namespace wd
{
class Buffer;

class ConsumerThread : public Thread
{
public:
    ConsumerThread(Buffer & buf);

    void run();
private:
    Buffer & buf_;
};

}//end of namespace wd

#endif

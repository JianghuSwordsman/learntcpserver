#ifndef WD_PRODUCERTHREAD_H_
#define WD_PRODUCERTHREAD_H_

#include "Thread.h"

namespace wd
{
class Buffer;

class ProducerThread : public Thread
{
public:
    ProducerThread(Buffer & buf);

    void run();
private:
    Buffer & buf_;
};

}//end of namespace wd

#endif

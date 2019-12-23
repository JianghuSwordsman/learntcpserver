#ifndef WD_THREADPOOLTHREAD_H_
#define WD_THREADPOOLTHREAD_H_

#include "Thread.h"

namespace wd
{
class Threadpool;

class ThreadpoolThread : public Thread
{
public:
    ThreadpoolThread(Threadpool & threadpool);

    void run();
private:
    Threadpool & threadpool_;
};
}//end of namespace wd
#endif

#ifndef WD_THREAD_H_
#define WD_THREAD_H_

#include "Thread.h"
#include "Noncopyable.h"
#include <pthread.h>
#include <functional>

namespace wd
{

class Thread : Noncopyable
{
public:
    typedef std::function<void()> ThreadCallback;
    Thread(ThreadCallback cb);

    void start();
    void stop();

    ~Thread();

    static void * threadFunc(void *);

private:
    pthread_t pthId_;
    bool isRunning_;
    ThreadCallback cb_;
};
}//end of namespace wd
#endif

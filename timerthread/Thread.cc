#include "Thread.h"


namespace wd
{
Thread::Thread(ThreadCallback cb)
    :cb_(cb),
    isRunning_(false),
    pthId_(0)
{}

void Thread::start()
{
    pthread_create(&pthId_,NULL,threadFunc,this);
    isRunning_ = true;
}

void Thread::stop()
{
    pthread_join(pthId_,NULL);
    isRunning_ = false;
}

Thread::~Thread()
{
    if(isRunning_)
        pthread_detach(pthId_);
}

void* Thread::threadFunc(void *arg)
{
    Thread * pThread = static_cast<Thread *>(arg);
    if(pThread)
    {
        pThread->cb_();
        return pThread;
    }
    else
        return NULL;
}
}//end of namespace wd

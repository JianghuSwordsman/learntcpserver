#include "Thread.h"
#include <iostream>

namespace wd
{

Thread::Thread()
   :pthId_(0),
    isRunning_(false)
{
   std::cout<<"Thread::Thread"<<std::endl;
}

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
void *Thread::threadFunc(void *arg)
{
    Thread * pThread = static_cast<Thread *>(arg);
    if(pThread != NULL)
    {
        pThread->run();
        return pThread;
    }
    return NULL;
}
Thread::~Thread()
{
    if(isRunning_)
    {
        pthread_detach(pthId_);
    }
    std::cout<<"Thread::~Thread"<<std::endl;

}
}//end of namespace wd

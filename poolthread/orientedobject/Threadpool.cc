#include "Threadpool.h"
#include "Buffer.h"
#include "ThreadpoolThread.h"
#include "Task.h"

namespace wd
{
Threadpool::Threadpool(size_t threadsNum,size_t buffNum)
    :threadsNum_(threadsNum),
    buf_(buffNum),
    isExit_(false)
{}

void Threadpool::start()
{
    for(size_t idx = 0; idx != threadsNum_; idx++)
    {
        Thread * pThread = new ThreadpoolThread(*this);;
        vecThreads_.push_back(pThread);
    }
    
    std::vector<Thread *>::iterator it;
    for(it = vecThreads_.begin(); it!= vecThreads_.end(); ++it)
    {
      (*it) ->start(); 
    }
}
void Threadpool::addTask(Task * ptask)
{
    buf_.push(ptask);
}

Task * Threadpool::getTask()
{
    return buf_.pop();
}

void Threadpool::threadFunc()
{
    while(!isExit_)
    {
        Task * ptask = getTask();
        if(ptask)
        {
            ptask->process();
        }
    } 
}

void Threadpool::stop()
{
    if(!isExit_)
    {
        isExit_ = true;
        buf_.wakeup_empty();
        buf_.set_flag(false);
        std::vector<Thread *>::iterator it;
        for(it = vecThreads_.begin(); it != vecThreads_.end(); it++)
        {
            (*it)->stop();
            delete (*it);
        }
    }
    vecThreads_.clear();
}

Threadpool::~Threadpool()
{
    if(!isExit_) 
        stop();
}
}//end of space wd

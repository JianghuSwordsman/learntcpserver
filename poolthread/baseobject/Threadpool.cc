#include "Threadpool.h"
#include "Buffer.h"
#include "Thread.h"
#include <functional>

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
        Thread * pThread = new Thread(std::bind(&Threadpool::threadFunc,this));
        vecThreads_.push_back(pThread);
    }
    
    std::vector<Thread *>::iterator it;
    for(it = vecThreads_.begin(); it!= vecThreads_.end(); ++it)
    {
      (*it)->start(); 
    }
}
void Threadpool::addTask(Task task)
{
    buf_.push(task);
}

void Threadpool::threadFunc()
{
    while(!isExit_)
    {
        Task task = buf_.pop();
        if(task)
        {
            task();
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

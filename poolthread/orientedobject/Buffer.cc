#include "Buffer.h"
//#include "Task.h"

namespace wd
{
Buffer::Buffer(size_t size)
    :   mutex_(),
        notFull_(mutex_),
        notEmpty_(mutex_),
        size_(size),
        flag_(true)
{
}

bool Buffer::full()
{
    return size_ == que_.size();
}

bool Buffer::empty()
{
    return 0 == que_.size();
}
void Buffer::push(Task *task)
{
    MutexLockGuard guard(mutex_);//局部对象
    //mutex_.lock();
    
    while(full())   
        notFull_.wait();
    que_.push(task);
    notEmpty_.notify();

    //mutex_.unlock();
}
Task * Buffer::pop()
{
    //mutex_.lock();
    MutexLockGuard guard(mutex_);//局部对象
    while(empty() && flag_)
        notEmpty_.wait();
    
    if(flag_)
    {
        Task * tmp = que_.front();
        que_.pop();
    
        notFull_.notify();
        
        //mutex_.unlock();
        return tmp;
    }
    else
        return NULL;
}
void Buffer::wakeup_empty()
{
    notEmpty_.notifyall();
}

void Buffer::set_flag(bool flag)
{
   flag_ =  flag;
}
}//end of namespace wd

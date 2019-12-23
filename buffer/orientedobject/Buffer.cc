#include "Buffer.h"

namespace wd
{
Buffer::Buffer(size_t size)
    :   mutex_(),
        notFull_(mutex_),
        notEmpty_(mutex_),
        size_(size)
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
void Buffer::push(int val)
{
    MutexLockGuard guard(mutex_);//局部对象
    //mutex_.lock();
    
    while(full())   
        notFull_.wait();
    que_.push(val);
    notEmpty_.notify();

    //mutex_.unlock();
}
int Buffer::pop()
{
    //mutex_.lock();
    MutexLockGuard guard(mutex_);//局部对象
    while(empty())
        notEmpty_.wait();

    int tmp = que_.front();
    que_.pop();

    notFull_.notify();
    
    //mutex_.unlock();
    return tmp;
}
}//end of namespace wd

#ifndef _MUTEXLOCK_H_
#define _MUTEXLOCK_H_

#include "Noncopyable.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define TINY_CHECK(exp)                                                                     \
    if(exp)                                                                                \
    {                                                                                       \
        fprintf(stderr,"File:%s Line:%d Exp:["#exp"] is true,abort.\n",__FILE__,__LINE__);  \
        abort();                                                                            \
    }

class MutexLock : private Noncopyable
{
    friend class Condition;    
public:
    MutexLock();
    ~MutexLock();
    void lock();
    void unlock();
    bool isLocking() const { return isLocking_; };
    pthread_mutex_t * getMutexPtr(){ return &mutex_; }

private:
    void restoreMutexStatus() { isLocking_ = true; }

    pthread_mutex_t mutex_;
    //是否上锁
    bool isLocking_;
};

class MutexLockGuard : Noncopyable
{
public:
    MutexLockGuard(MutexLock &mutex) : mutex_(mutex)
    { mutex_.lock(); }
    ~MutexLockGuard()
    { mutex_.unlock(); }
private:
    MutexLock &mutex_;
};
#endif

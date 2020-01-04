#include "Condition.h"
#include "MutexLock.h"
#include <assert.h>

Condition::Condition(MutexLock &mutex)
    :mutex_(mutex)
{
    TINY_CHECK(pthread_cond_init(&cond_, NULL));
}

Condition::~Condition()
{
    TINY_CHECK(pthread_cond_destroy(&cond_));
}

void Condition::wait()
{
    //wait前必须加锁，因为wait函数内部地原子操作包括解锁-等待-激活-重新加锁
    assert(mutex_.isLocking());
    TINY_CHECK(pthread_cond_wait(&cond_, mutex_.getMutexPtr()));
    mutex_.restoreMutexStatus();//感觉不必要
}

void Condition::notify()
{
    TINY_CHECK(pthread_cond_signal(&cond_));
}

void Condition::notifyall()
{
    TINY_CHECK(pthread_cond_broadcast(&cond_));
}

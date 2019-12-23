#ifndef WD_MUTEXLOCK_H_
#define WD_MUTEXLOCK_H_

#include <pthread.h>
#include "Noncopyable.h"

namespace wd
{
class MutexLock : public wd::Noncopyable
{
public:
    MutexLock();
    ~MutexLock();

    void lock();
    void unlock();

    pthread_mutex_t * getMutexLockPtr();

private:
    pthread_mutex_t mutex_;
    bool isLocked_;

};

class MutexLockGuard
{
public:
    MutexLockGuard(MutexLock & mutex);
    ~MutexLockGuard();
private:
    MutexLock & mutex_;
};
}//end of namespace wd
#endif

#ifndef WD_CONDITION_H_
#define WD_CONDITINO_H_

#include "Noncopyable.h"
#include <pthread.h>

namespace wd
{
class MutexLock;

class Condition : public Noncopyable
{
public:
    Condition(MutexLock & mutex);
    ~Condition();

    void wait();
    void notify();
    void notifyall();
private:
    pthread_cond_t cond_;
    MutexLock & mutex_;
};
}//end of namespace wd
#endif

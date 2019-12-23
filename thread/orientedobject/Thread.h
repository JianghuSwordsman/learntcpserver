#ifndef _WD_THREAD_H_
#define _WD_THREAD_H_

#include <pthread.h>

namespace wd
{

class Thread
{

public:
    Thread();
    ~Thread();
    virtual void run() = 0;
    void start();
    void stop();
private:
    static void* threadFunc(void*);
private:
    pthread_t pthId_;
    bool isRunning_;

};


}//end of namespace wd
#endif

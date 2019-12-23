#ifndef _WD_THREAD_H_
#define _WD_THREAD_H_

#include <pthread.h>
#include <functional>

namespace wd
{

class Thread
{

public:
    typedef std::function<void()> ThreadCallback;
    Thread(ThreadCallback cb);
    ~Thread();
    //virtual void run() = 0;
    void start();
    void stop();
private:
    static void* threadFunc(void*);
private:
    pthread_t pthId_;
    bool isRunning_;
    ThreadCallback cb_;
};


}//end of namespace wd
#endif

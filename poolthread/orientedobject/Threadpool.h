#ifndef WD_THREADPOOL_H_
#define WD_THREADPOOL_H_

#include "Threadpool.h"
#include <vector>
#include <stdio.h>
#include "Buffer.h"

namespace wd
{
class Thread;
class Task;
//class Buffer;
class Threadpool
{
    friend class ThreadpoolThread;
public:
    Threadpool(size_t threadsNum,size_t buffNum);

    void start();
    void stop();

    void addTask(Task *);
    ~Threadpool();
private:
    Task * getTask();

    void threadFunc();
private:
    size_t threadsNum_;
    std::vector<Thread*> vecThreads_;
    Buffer buf_;
    bool isExit_;
};
}//end of namespace wd
#endif

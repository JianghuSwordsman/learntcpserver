#ifndef WD_THREADPOOL_H_
#define WD_THREADPOOL_H_

#include <vector>
#include "Buffer.h"

namespace wd
{
class Thread;
//class Task;
//class Buffer;
class Threadpool
{
//class Thread;
public:
    typedef std::function<void()> Task;
    Threadpool(size_t threadsNum,size_t buffNum);

    void start();
    void stop();

    void addTask(Task);
    ~Threadpool();
private:
    void threadFunc();
private:
    size_t threadsNum_;
    std::vector<Thread*> vecThreads_;
    Buffer buf_;
    bool isExit_;
};
}//end of namespace wd
#endif

#ifndef _THREAD_H_
#define _THREAD_H_

#include "Noncopyable.h"
#include "Cache.h"
#include <functional>
#include <pthread.h>

class Thread : private Noncopyable
{
public:
    typedef std::function<void(Cache &)> ThreadCallback;
    explicit Thread(ThreadCallback callback, Cache &cache);
    ~Thread();

    void start();
    void join();
    //用于收集本线程地地cache
    Cache &get_cache();
private:
    static void *runInThread(void *);
private:
    pthread_t threadId_;
    bool isRunning_;
    //回调函数
    ThreadCallback callback_;
    //每个线程有自己地cache,定时器线程定时收集并同步所有线程地cache
    Cache cache_;
};
#endif

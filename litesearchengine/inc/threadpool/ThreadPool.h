#ifndef _THREADPOOL_H_
#define _THREADPOOL_H_

#include "MutexLock.h"
#include "Condition.h"
#include "Thread.h"
#include "Noncopyable.h"
#include "Cache.h"
#include <queue>
#include <vector>
#include <functional>
#include <memory>

class MyDict;
class Task;

class ThreadPool : private Noncopyable
{
public:
    ThreadPool(size_t queueSize, size_t threadsNum, Cache &cache, MyDict &mydict);
    //线程池开始
    void start();
    //往队列里添加任务对象
    void addTask(Task task);
    //从队列里获取任务对象，由线程执行体执行
    Task getTask();

    void runInThread(Cache &cache);//传递给线程执行体
    void update();//更新cache
private:
    //控制queue线程安全地push和pop
    mutable MutexLock mutex_;
    //添加task到queue后，需唤醒wait阻塞的消费者线程，此时任务队列非空
    Condition condc_;
    //从queue获取task后，需唤醒wait阻塞的生产者线程，此时任务队列非满
    Condition condp_;

    //任务队列及队列大小
    size_t queueSize_;
    std::queue<Task> queue_;

    //线程池线程只能指针及个数
    const size_t threadsNum_;
    std::vector<std::unique_ptr<Thread> > threads_;
    
    //线程池是否已经退出
    bool isExit_;
    //缓存
    Cache &cache_;
public:
    //字典，中文需离线分词
    MyDict &mydict_;

};
#endif

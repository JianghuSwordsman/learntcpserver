#include "ThreadPool.h"
#include "Task.h"
#include "MyDict.h"
#include <iostream>
#include <fstream>
#include <string>

ThreadPool::ThreadPool(size_t queueSize, size_t threadsNum, Cache &cache, MyDict &mydict)
    :condc_(mutex_),
    condp_(mutex_),
    queueSize_(queueSize),
    threadsNum_(threadsNum),
    isExit_(false),
    cache_(cache),
    mydict_(mydict)
{
}

void ThreadPool::start()
{
    for(size_t idx = 0; idx != threadsNum_; ++idx)
    {
        threads_.push_back(std::unique_ptr<Thread>(
                    new Thread(std::bind(&ThreadPool::runInThread,this,std::placeholders::_1),
                        cache_)));
    }
    for(size_t idx = 0; idx != threadsNum_; ++idx)
    {
        threads_[idx]->start();
    }
}

void ThreadPool::addTask(Task task)
{
    MutexLockGuard lock(mutex_);
    while(queue_.size() >= queueSize_) 
        condp_.wait();
    queue_.push(task);
    condc_.notify();
}

Task ThreadPool::getTask()
{
    MutexLockGuard lock(mutex_);
    while(queue_.empty())
        condc_.wait();
    Task task = queue_.front();
    queue_.pop();
    condp_.notify();
    return task;
}

void ThreadPool::runInThread(Cache &cache)
{
    while(!isExit_)
    {
        Task task(getTask());
        task.execute(cache);
    }
}

void ThreadPool::update()
{
    std::cout << "timer executint, update caches" << std::endl;
    
    for(size_t idx =0; idx != threadsNum_; ++idx)
    {
        cache_.add_elements(threads_[idx]->get_cache());
    }

    for(size_t idx = 0; idx != threadsNum_; ++idx)
    {
        threads_[idx]->get_cache().add_elements(cache_);
    }

    std::cout << "cache_.write_to_file();" << std::endl;
    std::string cachepath = "../data/cache.dat";
    std::ofstream ofs(cachepath.c_str());
    cache_.write_to_file(ofs);
}

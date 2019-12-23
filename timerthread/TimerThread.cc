#include "TimerThread.h"
#include "Timer.h"
#include "Thread.h"

namespace wd
{
TimerThread::TimerThread(int value,int interval,TimerCallback cb)
    :timer_(value,interval,cb),
    thread_(std::bind(&Timer::start,&timer_))
{}

void TimerThread::start()
{
    thread_.start();
}

void TimerThread::stop()
{
    timer_.stop();
    thread_.stop();
}
}//end of namesapce wd

#ifndef _MYTIMERTHREAD_H_
#define _MYTIMERTHREAD_H_

#include "Timer.h"
#include "TimerThread.h"

class MyTimerThread
{
public:
    typedef std::function<void()> Callback;

    MyTimerThread(int val, int interval, Callback cb);

    void start();
    void stop();
private:
    Timer timer_;
    TimerThread thread_;
};
#endif

#ifndef WD_TIMERTHRAED_H_
#define WD_TIMERTHREAD_H_

#include "Timer.h"
#include "Thread.h"
#include <functional>

namespace wd
{
class TimerThread
{

public:
    typedef std::function<void()> TimerCallback;
    TimerThread(int value,int interval,TimerCallback cb);

    void start();
    void stop();
private:
    Timer timer_;
    Thread thread_;
};
}//end of namesapce wd
#endif

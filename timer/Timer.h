#ifndef WD_TIMERFD_H_
#define WD_TIMERFD_H_

#include <functional>

namespace wd
{
class Timer
{
public:
    typedef std::function<void()> TimerCallback;
    Timer(int value,int interval,TimerCallback cb);
    void start();
    void stop();
    ~Timer();
private:
    int timerfd_;
    int value_;
    int interval_;
    TimerCallback cb_;
    bool isStarted_;
};
}//end of namespace wd
#endif

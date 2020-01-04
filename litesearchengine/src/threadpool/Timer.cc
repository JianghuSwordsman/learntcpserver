#include "Timer.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <poll.h>
#include <unistd.h>
#include <sys/timerfd.h>

int createTimerfd()
{
    int timerfd = ::timerfd_create(CLOCK_REALTIME,0);
    if(-1==timerfd)
    {
        perror("timerfd create error");
        exit(-1);
    }
    return timerfd;
}

void setTimerfd(int timerfd, int value,int interval)
{
    struct itimerspec val;
    memset(&val,0,sizeof(val));
    val.it_value.tv_sec = value;
    val.it_interval.tv_sec = interval;
    //第二个参数为0.本函数采用相对时间，至于传入哪个应该没影响，不知道怎么验证
    //r如果传入TFD_TIMER_ABCTIME,本函数采用绝对时间，获取时间可用clock_gettime,至于获取哪个时间
    //则取决于timerfd_create的参数
    if(-1 == ::timerfd_settime(timerfd, 0,&val, NULL))
    {
        perror("timerfd settime error");
        exit(-1);
    }
}

void stopTimerfd(int timerfd)
{
    setTimerfd(timerfd,0,0);
}

uint64_t readTimerfd(int timerfd)
{
    uint64_t howmany;
    if(::read(timerfd, &howmany, sizeof(howmany)) != sizeof(howmany))
    {
        perror("timerfd read error");
        exit(-1);
    }
    return howmany;
}

Timer::Timer(int val, int interval,Callback cb)
    :timerfd_(createTimerfd()),
    val_(val),
    interval_(interval),
    cb_(cb),
    isStarted_(false)
{}

Timer::~Timer()
{
    if(isStarted_)
    {
        stop();
        close(timerfd_);
    }
}

void Timer::start()
{
    setTimerfd(timerfd_, val_, interval_);
    isStarted_ = true;
    
    struct pollfd pfd;
    pfd.fd = timerfd_;
    pfd.events = POLLIN;

    int ret = 0;
    while(isStarted_)
    {
        ret = ::poll(&pfd,1,5000);
        if(-1 == ret)
        {
            if(errno == EINTR)
                continue;
            perror("poll error");
            exit(-1);
        }
        else if(0 == ret)
        {
            printf("epoll timeout.\n");
            continue;
        }

        if(POLLIN == pfd.revents)
        {
            uint64_t tmp = readTimerfd(timerfd_);
            (void)tmp;
            if(cb_)
                cb_();
        }
    }
}

void Timer::stop()
{
    isStarted_ = false;
    stopTimerfd(timerfd_);
}

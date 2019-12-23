#include "Timer.h"
#include <unistd.h>
#include <sys/timerfd.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <poll.h>

int createTimerfd()
{
    int timerfd = timerfd_create(CLOCK_REALTIME,0);
    if(-1 == timerfd)
    {
        perror("timerfd_create error");
    }
    return timerfd;
}

int setTimerfd(int timerfd,int value,int interval)
{
    struct itimerspec val;
    val.it_value.tv_sec = value;
    val.it_value.tv_nsec = 0;
    val.it_interval.tv_sec = interval;
    val.it_interval.tv_nsec = 0;

    int ret = timerfd_settime(timerfd,0,&val,NULL);
    if(-1 == ret)
    {
        perror("timerfd_settime error");
    }
    return ret;    
}

void stopTimerfd(int timerfd)
{
    setTimerfd(timerfd,0,0);
}

ssize_t readTimerfd(int timerfd)
{
    ssize_t howmany;
    ssize_t ret = read(timerfd,&howmany,sizeof(howmany));
    if(ret != sizeof(howmany))
    {
        perror("read timerfd error");
    }
    return howmany;
}


namespace wd
{
Timer::Timer(int value,int inrerval,TimerCallback cb)
    :timerfd_(createTimerfd()),
    value_(value),
    interval_(inrerval),
    cb_(cb)
{}

void Timer::start()
{
    setTimerfd(timerfd_,value_,interval_);

    struct pollfd pfd;
    pfd.fd = timerfd_;
    pfd.events = POLLIN;

    isStarted_ = true;
    while(isStarted_)
    {
        int ret = poll(&pfd,1,5000);
        if(-1 == ret)
        {
            if(errno == EINTR)
                continue;
            perror("poll error");
            exit(EXIT_FAILURE);
        }
        else if(0 == ret)
        {
            printf("poll timeout\n"); 
        }
        else
        {
            if(pfd.revents == POLLIN)
            {
                ssize_t howmany = readTimerfd(timerfd_);
                printf("howmany = %d\n",howmany);
                cb_();
            }
        }
    }
}

void Timer::stop()
{
    if(isStarted_)
    {
        stopTimerfd(timerfd_);
        isStarted_ = false;
    }
}

Timer::~Timer()
{
    if(isStarted_)
        stop();
}
}//end of namesapce wd

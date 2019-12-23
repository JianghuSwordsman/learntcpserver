#include "ThreadpoolThread.h"
#include "Threadpool.h"

namespace wd
{
ThreadpoolThread::ThreadpoolThread(Threadpool & threadpool)
    :threadpool_(threadpool)
{
}

void ThreadpoolThread::run()
{
    threadpool_.threadFunc();
}
}//end of namespace wd

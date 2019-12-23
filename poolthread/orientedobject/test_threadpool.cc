#include "Threadpool.h"
#include "Task.h"
#include <iostream>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

namespace wd
{
class MyTask : public Task
{
public:
    void process()
    {
        ::srand(time(NULL));
        int num = ::rand() % 100;
        std::cout << "producea number: " << num << std::endl;
    }
};
}//end of namespace wd

int main()
{
    wd::Threadpool threadpool(5,10);
    threadpool.start();

    wd::Task * pTask = new wd::MyTask;

    int count = 10;
    while(count--)
    {
            threadpool.addTask(pTask);
            ::sleep(1); 
    }
    threadpool.stop();
    
    return 0;
}


#include "Threadpool.h"
#include <time.h>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>

struct Task
{
    void operator()()
    {
        ::srand(::time(NULL));
        int num = ::rand() % 100;
        std::cout << "produce a number: " << num << std::endl;
    }
};

int main()
{
    wd::Threadpool threadpool(5,10);
    threadpool.start();
    int count = 20;
    while(count--)
    {
        threadpool.addTask(Task());
        ::sleep(1);
    }
    threadpool.stop();

    return 0;
}

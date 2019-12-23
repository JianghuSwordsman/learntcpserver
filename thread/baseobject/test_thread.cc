#include "Thread.h"
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <memory>
#include <time.h>
#include <functional>
//using namespace wd;

struct Run
{
void run()
{
    int i = 10;
    while(i--)
    {
        ::srand(::time(NULL));
        int num = rand() % 100;
        std::cout << "num = " << num << std::endl;
        ::sleep(1);        
    }    
}
};

int main()
{

    wd::Thread * pthread = new wd::Thread(std::bind(&Run::run,Run()));
    std::unique_ptr<wd::Thread> threadPtr(pthread);

    threadPtr->start();
    threadPtr->stop();

    return 0;
}



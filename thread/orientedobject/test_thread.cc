#include "Thread.h"
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <memory>
#include <time.h>

//using namespace wd;

class MyThread : public wd::Thread
{
public:
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

    wd::Thread * pthread = new MyThread;
    std::unique_ptr<wd::Thread> threadPtr(pthread);

    threadPtr->start();
    threadPtr->stop();

    return 0;
}



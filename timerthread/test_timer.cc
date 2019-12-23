#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include "TimerThread.h"

void getrandom()
{
    ::srand(::time(NULL));
    int num = ::rand() % 100;

    std::cout << "produce a number: " << num << std::endl;
}

int main()
{
    wd::TimerThread timer(1,6,getrandom);
    timer.start();
    sleep(20);
    timer.stop();

    return 0;
}

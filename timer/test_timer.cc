#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include "Timer.h"

void getrandom()
{
    ::srand(::time(NULL));
    int num = ::rand() % 100;

    std::cout << "produce a number: " << num << std::endl;
}

int main()
{
    wd::Timer timer(1,6,getrandom);
    timer.start();
    timer.stop();

    return 0;
}

#include "ConsumerThread.h"
#include "Buffer.h"
#include <unistd.h>
#include <iostream>

namespace wd
{
ConsumerThread::ConsumerThread(Buffer & buf)
    :buf_(buf)
{}

void ConsumerThread::run()
{
    while(1)
    {
        int num = buf_.pop();
        std::cout<< "Consume a number: " << num << std::endl;
        //::sleep(2);
    }
}
}//end of namespace wd

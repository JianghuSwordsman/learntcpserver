#include "ProducerThread.h"
#include "Buffer.h"
#include <unistd.h>
#include <stdlib.h>

namespace wd
{
ProducerThread::ProducerThread(Buffer & buf)
    :buf_(buf)
{
}

void ProducerThread::run()
{
    while(1)
    {
        ::srand(::time(NULL));
        int num  = ::rand() % 100;
        buf_.push(num);
        
        //::sleep(1);
    }
}
}//end of namespace wd

#include "ProducerThread.h"
#include "ConsumerThread.h"
#include "Buffer.h"
#include <memory>
//#include <iostream>
//#include <stdlib.h>
using namespace wd;

int main()
{
    Buffer buffer(10);
    Thread *pProducer = new ProducerThread(buffer);
    Thread *pConsumer = new ConsumerThread(buffer);
    std::unique_ptr<Thread> producer(pProducer);
    std::unique_ptr<Thread> consumer(pConsumer);

    producer->start();
    consumer->start();

    producer->stop();
    consumer->stop();

    return 0;
}

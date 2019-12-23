#include "Buffer.h"
#include <memory>
#include <iostream>
#include "Thread.h"
#include <stdlib.h>
#include <time.h>
#include <unistd.h>


using namespace wd;

struct Producer
{
    void run(Buffer& buf)
    {
        while(1)
        {
            ::srand(::time(NULL));
            int num = ::rand() % 100;
            buf.push(num);
            std::cout << "produce a number: " << num << std::endl;
            ::sleep(1);        
        }
    }

};
struct Consumer
{
    void run(Buffer& buf)
    {
        while(1)
        {
            int num = buf.pop();
            std::cout << "consume a number: " << num << std::endl;
            ::sleep(2);        
        }
    }
};

int main()
{
    Buffer buffer(10);
    Thread producer(std::bind(&Producer::run,Producer(),std::ref(buffer)));
    Thread consumer(std::bind(&Consumer::run,Consumer(),std::ref(buffer)));

    producer.start();
    consumer.start();

    producer.stop();
    consumer.stop();

    return 0;
}

#ifndef WD_BUFFER_H_
#define WD_BUFFER_H_

#include "Noncopyable.h"
#include "MutexLock.h"
#include "Condition.h"
#include <queue>
namespace wd
{
class Buffer : public Noncopyable
{
public:
    Buffer(size_t size);

    void push(int);
    int pop();

    bool full();
    bool empty();
private:
    MutexLock           mutex_;
    Condition           notFull_;
    Condition           notEmpty_;
    std::queue<int>     que_;
    size_t              size_;

};

}//end of namespace wd
#endif

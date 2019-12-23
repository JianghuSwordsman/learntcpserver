#ifndef WD_BUFFER_H_
#define WD_BUFFER_H_

#include "Noncopyable.h"
#include "MutexLock.h"
#include "Condition.h"
#include <queue>
namespace wd
{
class Task;

class Buffer : public Noncopyable
{
public:
    Buffer(size_t size);

    void push(Task *);
    Task *pop();

    bool full();
    bool empty();
    void wakeup_empty();
    void set_flag(bool flag);
private:
    MutexLock           mutex_;
    Condition           notFull_;
    Condition           notEmpty_;
    std::queue<Task *>    que_;
    size_t              size_;
    bool                flag_;

};

}//end of namespace wd
#endif

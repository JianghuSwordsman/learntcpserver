#ifndef WD_NONCOPYABLE_H_
#define WD_NONCOPYABLE_H_

class Noncopyable
{
protected:
    Noncopyable(){};
private:
    Noncopyable(const Noncopyable & rhs);
    Noncopyable & operator=(const Noncopyable & rhs);
};
#endif

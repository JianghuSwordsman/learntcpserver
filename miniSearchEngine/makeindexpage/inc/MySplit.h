#ifndef _MYSPLIT_H_
#define _MYSPLIT_H_

#include <string>
#include <vector>

class MyConf;

class MySplit
{
public:
    MySplit(MyConf & conf);
    ~MySplit();
    //分词，把非停留词放到vector返回，此处用到的停留词列表在conf中，但是public成员，感觉放到DealPage会好一点，通过传参传递
    std::vector<std::string>  operator()(const std::string &str);
private:
    MyConf & conf_;
};
#endif


#ifndef _MYCONF_H_
#define _MYCONF_H_

#include <string>
#include <sstream>
#include <map>
#include <ext/hash_map>

class MyHashFn
{
public:
    std::size_t operator()(const std::string & str)const
    {
        return __gnu_cxx::__stl_hash_string(str.c_str());
    }
};

class MyConf
{
public:
    //读取指定目录配置文件的配置信
    MyConf(std::string &);
    //读取配置文件到map
    bool init();

    std::map<std::string, std::string> getMap()
    {
        return map_;
    }
    void show();
public:
    //stoplist，关联词列表,总感觉public对象成员不太好，其实可以放到DealPage中
    __gnu_cxx::hash_map<std::string, std::string, MyHashFn> stop_;
    //文档偏移信息，docid offset size.已经放到DealPage.h
    //__gnu_cxx::hash_map<int, std::pair<int, int>, MyHashFn> offset_;
private:
    //配置文件路径
    std::string filepath_;
    //存储配置信息项和值
    std::map<std::string, std::string> map_;
};
#endif

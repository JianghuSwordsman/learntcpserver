#ifndef _CACHE_H_
#define _CACHE_H_

#include "MutexLock.h"
#include <ext/hash_map>
#include <string>

struct MyHashFn
{
    std::size_t operator()(const std::string &str) const
    {
        return __gnu_cxx::__stl_hash_string(str.c_str());
    }
};

class Cache
{
public:
    typedef __gnu_cxx::hash_map<std::string, std::string, MyHashFn> HashMap;
    typedef __gnu_cxx::hash_map<std::string, std::string, MyHashFn>::iterator HashMap_it;
    typedef __gnu_cxx::hash_map<std::string, std::string, MyHashFn>::const_iterator HashMap_cit;

    Cache(const int num = 100);
    Cache(const Cache &cache);
    //把key-value添加到hashmap
    void map_to_cache(std::string &key, std::string &value);
    //通过key查询value，无则返回空字符串
    std::string query_cache(const std::string &word);
    //把cache内存写到cache文件
    void write_to_file(std::ofstream &fout);
    //从cache文件都区到内存cache
    void read_from_file(const std::string &filename);
    //把参数cache中元素加到本对象cache,用于cache地收集和同步到各线程
    void add_elements(const Cache &cache);
private:
    HashMap hashmap_;
    MutexLock mutex_;
};
#endif

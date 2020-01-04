#include "Cache.h"
#include <utility>
#include <iostream>
#include <fstream>

Cache::Cache(const int num)
    :hashmap_(num),
    mutex_()
{}

Cache::Cache(const Cache &cache)
    :hashmap_(cache.hashmap_),
    mutex_()
{}

void Cache::map_to_cache(std::string &key, std::string &value)
{
    MutexLockGuard guard(mutex_);
    hashmap_[key] = value;
}

std::string Cache::query_cache(const std::string &word)
{
    HashMap_it iter = hashmap_.find(word);
    if(iter != hashmap_.end())
    {
        return iter->second;
    }
    else
    {
        return std::string();
    }
}

void Cache::write_to_file(std::ofstream &fout)
{
    MutexLockGuard guard(mutex_);
    HashMap_it iter;
        for(iter = hashmap_.begin(); iter != hashmap_.end(); ++iter)
        {
            fout << iter->first << "\t" << iter->second << std::endl;
        }
}

void Cache::read_from_file(const std::string &filename)
{
    std::ifstream fin(filename.c_str());
    if(!fin)
    {
        std::cout<< "open cache file failed!\n" << std::endl;
        return;
    }

    std::string key,value;
    while(fin >> key >> value)
    {
        hashmap_.insert(make_pair(key,value));
        std::cout << key << " " << value << std::endl;
    }
}

void Cache::add_elements(const Cache &cache)
{
    MutexLockGuard guard(mutex_);
    HashMap_cit citer = cache.hashmap_.begin();
    for(; citer != cache.hashmap_.end(); ++citer)
    {
        hashmap_.insert(*citer);
    }
}

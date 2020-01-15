#include "MyConf.h"
#include <iostream>
#include <fstream>
#include <utility>
#include <stdlib.h>

MyConf::MyConf(std::string &path)
    :filepath_(path)
{
    init();
}

bool MyConf::init()
{
    std::ifstream ifs(filepath_.c_str());
    if(!ifs)
    {
        std::cout << "file read error" << std::endl;
        exit(EXIT_FAILURE);
    }
    std::string line;
    while(getline(ifs,line))
    {
        std::istringstream iss(line);
        std::string key,value;
        iss >> key >> value;
        //std::cout << "key:" << key << "value:" << value << std::endl;
        map_.insert(make_pair(key,value));
    }
    ifs.close();
    ifs.clear();
#if 1
    ifs.open(map_["my_stoplist"].c_str());
    if(!ifs)
    {
        std::cout << "file read stoplist error" << std::endl;
        exit(EXIT_FAILURE);
    }
    while(ifs >> line)
    {
        stop_[line] = line;
    }
    ifs.close();
    ifs.clear();
#endif
    return true;
}

void MyConf::show()
{
    std::map<std::string, std::string>::const_iterator mcit = map_.begin();
    for(; mcit != map_.end(); ++mcit)
    {
        std::cout << mcit->first << "-->" << mcit->second << std::endl;
    }
}

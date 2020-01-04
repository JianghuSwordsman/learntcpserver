#include "MyConf.h"
#include <iostream>
#include <fstream>
#include <utility>

MyConf::MyConf(std::string &path)
    :filepath_(path)
{}

bool MyConf::init()
{
    std::ifstream ifs(filepath_.c_str());
    if(!ifs)
    {
        std::cout << "file read error" << std::endl;
        return false;
    }
    std::string line;
    while(getline(ifs,line))
    {
        std::istringstream iss(line);
        std::string key,value;
        iss >> key >> value;
        map_.insert(make_pair(key,value));
    }
    ifs.close();
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

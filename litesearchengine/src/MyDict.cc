#include "MyDict.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

MyDict *MyDict::pInstance_ = NULL;

MyDict * MyDict::createInstance(const char *dictfilepath, const char *indexfilepath)
{
    if(NULL == pInstance_)
    {
        pInstance_ = new MyDict(dictfilepath, indexfilepath);
    }
    return pInstance_;
}

std::vector<std::pair<std::string, int> > & MyDict::get_dict()
{
    return dict_;
}

std::map<std::string,std::set<int> > & MyDict::get_index_table()
{
    return index_table_;
}

void MyDict::show_dict()
{
    for(auto iter = dict_.begin(); iter != dict_.end(); ++iter)
    {
        std::cout << iter->first << "-->" << iter->second << std::endl;
    }

}

void MyDict::show_index_table()
{
    for(auto iter = index_table_.begin(); iter != index_table_.end(); ++iter)
    {
        std::cout << iter->first << "-->";
        for(auto sit = iter->second.begin(); sit != iter->second.end(); ++sit)
        {
            std::cout << *sit << " ";
        }
        std::cout << std::endl;
    }
}

void MyDict::read_from(const char * filepath)
{
    std::ifstream in(filepath);
    if(!in)
    {
        std::cout << __DATE__ << " " << __TIME__
                  << __FILE__ << " " << __LINE__
                  << " : dict READ ERROR" << std::endl;

        exit(-1);
    }

    std::string line;
    while(getline(in,line))
    {
        std::stringstream ss(line);
        std::string key;
        int value;
        ss >> key >> value;
        dict_.push_back(make_pair(key,value));       
    }
    in.close();
}

MyDict::MyDict(const char *dictfilepath, const char *indexfilepath)
{
    read_from(dictfilepath);
    read_from(indexfilepath);

    //创建索引
    for(size_t idx = 0; idx != dict_.size(); ++idx)
    {
        record_to_index(idx);
    }
}

void MyDict::record_to_index(int idx)
{
    std::string key;
    std::string word = dict_[idx].first;
    //std::cout << "word: " << word << " size: " << word.size() << std::endl;
    for(std::size_t iidx = 0; iidx != word.size(); ++iidx)
    {
        //UTF-8中文长度识别
        char ch =word[iidx];
        if(ch & (1 << 7))
        {
            if((ch & 0xF0) == 0xC0 || (ch & 0xF0) == 0xD0)
            {
                key = word.substr(iidx,2);
                ++iidx;
            }
            else if((ch & 0xF0) == 0xE0)
            {
                key = word.substr(iidx,3);
                iidx += 2;
            }
            else if((ch & 0xF0) == 0xF0)
            {
                key = word.substr(iidx,4);
                iidx +=3;
            }
            else if((ch & 0xFF) == 0xF8 )
            {
                key= word.substr(iidx,5);
                iidx += 4;
            }
            else if((ch & 0xFF) == 0xFE)
            {
                key = word.substr(iidx,6);
                iidx += 5;
            }
            else
            {
                std::cout << "MyDict.cc file format is not utf-8" << std::endl;
            }
            //std::cout << "Ciidx: " << iidx << " ch: " << std::hex << (int)ch << std::endl;
        }
        else
        {
            //ASICC
            //std::cout << "Eiidx: " << iidx << " ch: " << std::hex << (int)ch << std::endl;
            key = word.substr(iidx,1);
        }
        index_table_[key].insert(idx);
    }
}



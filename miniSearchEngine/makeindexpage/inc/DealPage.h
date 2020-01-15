#ifndef _DEALPAGE_H_
#define _DEALPAGE_H_

#include "MySplit.h"
#include "Page.h"
#include "MyConf.h"
#include <vector>
#include <string>
#include <ext/hash_map>
#include <set>

#if 0
class MyHashFn
{
public:
    std::size_t operator()(const std::string & str)const
    {
        return __gnu_cxx::__stl_hash_string(str.c_str());
    }
};

#endif

class DealPage
{
public:
    //传入配置文件路径，用于初始化MyConf
    DealPage(std::string &filepath);
    //创建网页.去重，计算权重，更新偏移文件，建立倒排索引
    void createPage();
    //读取网页
    void readpage(__gnu_cxx::hash_map<int, Page> & pagemap);
    //读取倒排索引
    void readinvertindex(__gnu_cxx::hash_map<std::string, std::set<std::pair<int, double> >, MyHashFn> & invertindex);
private:
    //网页去重
    void removeDuplication(std::vector<Page> &pagevec);
    //建立倒排索引，无权重，待计算
    void invertIndex(std::vector<Page> &pagevec, std::map<std::string, std::vector<std::pair<int,int> > > & indexmap);
private:
    MyConf conf_;
    MySplit split_;
    //文档在page中的docid offset size
    __gnu_cxx::hash_map<int, std::pair<int, int > > offset_;
    //倒排索引
    //__gnu_cxx::hash_map<std::string, std::vector<std::pair<int, double> >, MyHashFn> invertindex_;
    //page
    //std::vector<page>  pagevec_;
};
#endif

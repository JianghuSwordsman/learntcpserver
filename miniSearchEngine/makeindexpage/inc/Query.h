#ifndef _QUERY_H_
#define _QUERY_H_

#include "MySplit.h"
#include "Page.h"
#include "MyConf.h"
#include <vector>
#include <string>
#include <ext/hash_map>
#include <set>
//重载函数运算符，用于排序函数，计算余弦相似度
class Compare
{
public:
    Compare(std::vector<double> & weightvec);
    bool operator()(const std::pair<int, std::vector<double> > & left, const std::pair<int, std::vector<double> > &right);
private:
    std::vector<double> &vec_;
};

class Query
{
public:
    //传入配置文件路径，用于初始化MyConf
    Query(std::string &filepath);
    //对于从客户端接受到的词条执行查询，把词条当网页看待，计算权重，并计算与page网页相似度,择优返回
    std::string pagequery(const std::string & strbuf);
private:
    //读取网页
    void loadpage();
    //读取倒排索引
    void loadinvertindex();
    //读取偏移文件
    void loadnewoffset();
    //查询
    bool findpage(std::vector<std::string> &queryvec, std::vector<std::pair<int, std::vector<double> > > &resvec);
private:
    //配置文件信息
    MyConf conf_;
    //分词对象，重载了函数运算符
    MySplit split_;
    //网页相似度比较对象，通过计算余弦相似度
    //Comparepage comparepage_;

    //文档在page中的docid offset size
    __gnu_cxx::hash_map<int, std::pair<int, int > > offset_;
    //倒排索引
    __gnu_cxx::hash_map<std::string, std::set<std::pair<int, double> >, MyHashFn> invertindex_;
    //docid page
    __gnu_cxx::hash_map<int, Page> pagemap_;
};

#endif

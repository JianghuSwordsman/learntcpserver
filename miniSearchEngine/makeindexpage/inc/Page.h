#ifndef _PAGE_H_
#define _PAGE_H_

#include <string>
#include <vector>
#include <utility>
#include <map>
#include <iostream>

class MyCompare
{
public:
    bool operator()(const std::pair<std::string, int> & lhs, const std::pair<std::string, int> & rhs)  
    {
        if(lhs.second < rhs.second)
        {
            return true;
        }
        else if(rhs.second == rhs.second && lhs.first > rhs.first)
        {
            return true;
        }
        else
            return false;
    }
};
class MySplit;
class MyConf;

class Page
{
    //判断两个网页是否相等，如果词频top10的，有6个相同，认为两篇文档相同
    friend bool operator ==(const Page &lhs, const Page &rhs);
public:
    Page(){}
    //创建page
    Page(const std::string  & docstr,const MyConf & myconf, MySplit & split);

    int getdocid()
    {
        //std::cout << "docid_:" << docid_ << std::endl;
        return std::stoi(docid_);
    }
    //生成文档摘要。
    //根据查询词，在文档中按行匹配，有查询词的行作为，摘要的一部分,最多五行
    std::string summary(std::vector<std::string> &queryvec);

private:
    //解析网页，对文档内容分词，生成词频top10的词的vector
    void docsplit(const std::string & docstr, const MyConf & myconf, MySplit & split);
    //生成词频top10的词的vector
    void createtop10(const std::vector<std::string> & wordVec);
private:
    //文档id
    std::string docid_;
    //文档url
    //std::string docurl_;
    //文档内容
    std::string doccontent_;
    //文档分词去停留词之后的top10,代表一篇文档的特征，
    std::vector<std::string> topstrvec_;
public:
    //去停留词之后的词和词频
    std::map<std::string, int> wordfrequency_;    
    //文档主题
    std::string doctitle_;
};
#endif

#include "Query.h"
#include "Page.h"
#include "MyConf.h"
#include "MyJson.h"
#include <stdlib.h>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <math.h>
#include <fstream>

Compare::Compare(std::vector<double> & weightvec)
    :vec_(weightvec)
{}

//计算余弦相似度，越大认为两个网页越相似
bool Compare::operator ()(const std::pair<int, std::vector<double> > &left, const std::pair<int, std::vector<double> > &right)
{
    double weightleft = 0, weightright;
    double powleft = 0, powright = 0;
    int idx;
    for(idx = 0; idx != vec_.size(); idx++)
    {
        weightleft += (left.second)[idx] * vec_[idx];
        weightright += (right.second)[idx] * vec_[idx];
        powleft += pow((left.second)[idx], 2);
        powright += pow((right.second)[idx], 2);
    }
    if(weightleft / sqrt(powleft) < weightright / sqrt(powright))
    {
        return false;
    }
    else
    {
        return true;
    }   
}

Query::Query(std::string & filepath)
    :conf_(filepath),
    split_(conf_)
{
    //加载偏移信息库
    loadnewoffset();
    //加载网页库网页
    loadpage();
    //加载倒排索引
    loadinvertindex();
}

void Query::loadnewoffset()
{
    std::ifstream ifs(conf_.getMap()["my_newoffset"]);
    if(!ifs)
    {
        std::cout << "readpage  open offset file error" << std::endl;
        exit(EXIT_FAILURE);
    }
    int docid, offset, size;
    std::string line;
    while(getline(ifs, line))
    {
        std::istringstream iss(line);
        iss >> docid >> offset >> size;
        offset_[docid].first = offset;
        offset_[docid].second = size;
    }
    ifs.close();
}

void Query::loadpage()
{
    std::ifstream ifs(conf_.getMap()["my_page"]);
    if(!ifs)
    {
        std::cout << "readpage open page file error" << std::endl;
        exit(EXIT_FAILURE);
    }
    
    int offsetnum = 0;
    std::string textstr;
    textstr.resize(1024*100);
    //offset的键值是docid，所以只能从1开始
    int id = 1;
    int maxsize = offset_.size();
    while(!ifs.eof() && id <= 10000 && id <= maxsize)
    {

        if(offset_[id].second > 102400)
            textstr.resize(offset_[id].second);

        ifs.seekg(offsetnum).read(&textstr[0], offset_[id].second);
        
        Page page(textstr, conf_, split_);
        pagemap_[page.getdocid()] = page;

        offsetnum += offset_[id].second;
        id++;
    }
    ifs.close();
}

void Query::loadinvertindex()
{
    std::ifstream ifs(conf_.getMap()["my_invertindex"]);
    if(!ifs)
    {
        std::cout << "readinvertindex open invertindex file error" << std::endl;
        exit(EXIT_FAILURE);
    }
    int docid, frequency;
    double weight;
    std::string line;
    while(getline(ifs, line))
    {
        std::istringstream iss(line);
        std::string key;
        iss >> key;
        while(iss >> docid >> frequency >> weight)
        {
            invertindex_[key].insert(std::make_pair(docid, weight));
        }
    }
    ifs.close();
}
std::string Query::pagequery(const std::string & strbuf)
{
    //存储待查询词条分词之后的词语
    std::vector<std::string> queryvec;
    //分词
    queryvec = split_(strbuf);
    //存储待查寻词条的词和词频
    std::map<std::string, int> freqmap;
    //统计词频
    for(auto iter = queryvec.begin(); iter != queryvec.end(); iter++)
    {
        freqmap[*iter]++;
    }

    //存储待查寻词条中每个词的权重，同一个词多次出现，则多次计算
    std::vector<double> weightvec;
    //权重平方和
    double weightsum = 0;
    //网页库文档总数目
    int pagenum = offset_.size();
    //计算每篇文档中词的权重，并归一化处理
    for(auto iter = queryvec.begin();  iter != queryvec.end(); iter++)
    {
        int df = invertindex_[*iter].size();
        double idf = log((double)pagenum/df + 0.05)/log(2);
        int tf = freqmap[*iter];

        weightsum += pow(tf * idf, 2);
        weightvec.push_back(tf * idf);
    }
    //计算每个查询词的权重
    for(auto iter = weightvec.begin(); iter != weightvec.end(); iter++)
    {
        *iter /= sqrt(weightsum) ;
    }
    
    //保存查询结果,docid 各查询词的权重
    std::vector<std::pair<int, std::vector<double> > > resvec;
    //对待查寻词条的每个词去查询网页库，包括所有查询词的网页为预返回网页，
    //返回相应网页的权重
    if(findpage(queryvec, resvec))
    {
        Compare com(weightvec);
        stable_sort(resvec.begin(),resvec.end(), com);

        MyJson ajson;

        return ajson(resvec, pagemap_, queryvec);
    }
    else
    {
        MyJson ajson;
        return ajson.noresult();
    }

}

bool Query::findpage(std::vector<std::string> &queryvec, std::vector<std::pair<int, std::vector<double> > > &resvec)
{
    auto iter = queryvec.begin();
    for(; iter != queryvec.end(); iter++)
    {
        int num = invertindex_.count(*iter);
        if(num == 0)
        {
            break;
        }
    }
    //当查询词为空，或者某个词查询不到，则查询失败，文档不存在
    if(iter != queryvec.end() || queryvec.size() == 0)
    {
        return false;
    }
    else
    {
        typedef std::set<std::pair<int, double> >::iterator set_iter;
        typedef std::pair<set_iter, int> setierpair;
        //迭代器数组，以及迭代器当前所处的位置，数组长度等于查询词个数,
        //指向每个查询词对应的倒排索引对应的pair,pair中存储的是docid和词在对应文档中的权重
        setierpair * iterarr = new setierpair[queryvec.size()];
        int minsize = 0xFFFFFF;
        //给每个迭代器赋值，并求出所有迭代器指向的set的最小长度
        int idx = 0;
        for(auto iter =queryvec.begin(); iter != queryvec.end(); iter++)
        {
            iterarr[idx].first = invertindex_[*iter].begin();
            iterarr[idx].second = 0;
            idx++;
            minsize = minsize < invertindex_[*iter].size() ? minsize : invertindex_[*iter].size();
        }
        //exitflag 为true退出while循环
        bool exitflag = false;
        //比较每个pair数组中每个迭代器对应的docid，文档中同时拥有所有查询词的docid才是预期返回的page，
        //如果当前迭代器指向的docid不同，则迭代器向前移动，对应pair中的位置+1
        //直到当前位置等于迭代器指向的set的最小的size
        while(!exitflag)
        {
            for(idx = 0; idx < queryvec.size() - 1; idx++)
            {
                if(iterarr[idx].first->first != iterarr[idx+1].first->first)
                    break;
            }
            //以为当前迭代器数组中的迭代器指向的docid相同，命中预期返回page
            if(idx == queryvec.size() - 1)
            {
                std::vector<double> weightvec;
                //保存当前docid
                int docid = iterarr[0].first->first;
                for(idx = 0; idx != queryvec.size(); idx++)
                {
                    weightvec.push_back(iterarr[idx].first->second);
                    iterarr[idx].first ++;
                    iterarr[idx].second ++;
                    if(iterarr[idx].second == minsize)
                    {
                        exitflag = true;
                    }
                }
                resvec.push_back(make_pair(docid, weightvec));
            }
            else
            {   
                //保存所有迭代器指向的最小的docid，最小的对应迭代器前移
                int mindocid = 0xFFFFF;
                //报错最小的docid对应的下标位置
                int minindex;
                for(idx = 0; idx != queryvec.size(); idx++)
                {
                    if(iterarr[idx].first->first < mindocid)
                    {
                        mindocid = iterarr[idx].first->first;
                        minindex = idx;
                    }
                }
                iterarr[minindex].first++;
                iterarr[minindex].second++;
                if(iterarr[minindex].second == minsize)
                {
                    exitflag = true;
                }
            }
        }//end while

        if(queryvec.size() == 1)
        {
            delete iterarr;
        }
        else
        {
            delete []iterarr;
        }
        return true;
    }//end of else
}//end of func

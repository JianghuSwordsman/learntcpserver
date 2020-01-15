#include "DealPage.h"
#include "Page.h"
#include "MyConf.h"
#include <stdlib.h>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <math.h>
#include <fstream>

DealPage::DealPage(std::string & filepath)
    :conf_(filepath),
    split_(conf_)
{
    //conf_.show();

    std::ifstream ifs(conf_.getMap()["my_offset"]);
    if(!ifs)
    {
        std::cout << "DealPage open offset file error" << std::endl;
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
void DealPage::readpage(__gnu_cxx::hash_map<int, Page> & pagemap)
{
    std::ifstream ifs(conf_.getMap()["my_newoffset"]);
    if(!ifs)
    {
        std::cout << "readpage  open offset file error" << std::endl;
        exit(EXIT_FAILURE);
    }
    int docid, offset, size;
    std::string line;
    __gnu_cxx::hash_map<int, std::pair<int, int> > offsetmap;
    while(getline(ifs, line))
    {
        std::istringstream iss(line);
        iss >> docid >> offset >> size;
        offsetmap[docid].first = offset;
        offsetmap[docid].second = size;
    }
    ifs.close();
    ifs.clear();

    ifs.open(conf_.getMap()["my_page"]);
    if(!ifs)
    {
        std::cout << "readpage open page file error" << std::endl;
        exit(EXIT_FAILURE);
    }
    
    offset = 0;
    std::string textstr;
    textstr.resize(1024*100);
    //offset的键值是docid，所以只能从1开始
    int id = 1;
    int maxsize = offset_.size();
    while(!ifs.eof() && id <= 10000 && id <= maxsize)
    {

        if(offset_[id].second > 102400)
            textstr.resize(offset_[id].second);

        ifs.seekg(offset).read(&textstr[0], offsetmap[id].second);
        
        Page page(textstr, conf_, split_);
        pagemap[page.getdocid()] = page;

        offset += offsetmap[id].second;
        id++;
    }
}

void DealPage::readinvertindex(__gnu_cxx::hash_map<std::string, std::set<std::pair<int ,double> >,MyHashFn> & invertindex)
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
            invertindex[key].insert(std::make_pair(docid, weight));
        }
    }
    ifs.close();
    ifs.clear();    
}
void DealPage::createPage()
{
    //读取网页信息，其中包括，解析page,分词，去停留词
    std::vector<Page> pagevec;
    std::ifstream ifs(conf_.getMap()["my_page"]);
    if(!ifs)
    {
        std::cout << "createPage open file error" << std::endl;
        exit(EXIT_FAILURE);
    }
    int offset = 0;
    std::string textstr;
    textstr.resize(1024*100);
    //offset_中的键值是 docid所以，id也只能从1开始
    int id = 1;
    int maxsize = offset_.size();
    while(!ifs.eof() && id <= 10000 && id <= maxsize)
    {
        std::cout << "id:" << id << std::endl;
        //std::cout << "offset_.size():" << offset_.size() << "offset_[id].second" << offset_[id].second << std::endl;
        if(offset_[id].second > 102400)
            textstr.resize(offset_[id].second);
        ifs.seekg(offset).read(&textstr[0],offset_[id].second);        
        Page page(textstr, conf_, split_);
        pagevec.push_back(page);

        offset += offset_[id].second;
        id++;
    }

    //网页去重
    removeDuplication(pagevec);

    //将去重之后的文档偏移信息写入新文件
    std::ofstream ofs(conf_.getMap()["my_newoffset"]);
    if(!ofs)
    {
        std::cout << "createPage open newoffset file error" << std::endl;
        exit(EXIT_FAILURE);
    }
    int pageindex;
    for(pageindex = 0; pageindex !=pagevec.size(); pageindex++)
    {
        ofs << pagevec[pageindex].getdocid() << "\t" << offset_[pagevec[pageindex].getdocid()].first << "\t" << offset_[pagevec[pageindex].getdocid()].second << std::endl;        
    }
    ofs.close();
    ofs.clear();

    //建立倒排索引
    //word docid 词频 权重(待计算)
    std::map<std::string, std::vector<std::pair<int, int> > > indexmap;
    std::map<std::string, std::vector<std::pair<int, int> > >::iterator iterInvert;
    invertIndex(pagevec, indexmap);

    std::map<int, double> sumMap;
    //计算每篇文档中词的权重，并归一化处理
    //遍历倒排索引，求出每篇文档的所有词的权重的平方和，用于归一化处理
    for(iterInvert = indexmap.begin(); iterInvert != indexmap.end(); iterInvert++)
    {
        int df = (iterInvert->second).size();
        int pagenum = pagevec.size();
        double idf = log((double)pagenum/df + 0.05)/log(2);
        std::vector<std::pair<int, int> >::iterator veciter;
        for(veciter = (iterInvert->second).begin(); veciter != (iterInvert->second).end(); veciter++)
        {
            sumMap[veciter->first] += pow((veciter->second)*idf, 2);
        }
    }
    ofs.open(conf_.getMap()["my_invertindex"]);
    if(!ofs)
    {
        std::cout << "createPage open invertindex  file error" << std::endl;
        exit(EXIT_FAILURE);
    }
    //将每个词在每篇文档中的权重添加到倒排索引中，并写入文件
    //word docid1 frequency1 weight1 docid2 frequency2 weight2...
    for(iterInvert = indexmap.begin(); iterInvert != indexmap.end(); iterInvert++)
    {
        ofs << iterInvert->first << "\t";
        int df = (iterInvert->second).size();
        int pagenum = pagevec.size();
        double idf = log((double)pagenum/df + 0.05) / log(2);
        std::vector<std::pair<int, int> >::iterator veciter;
        for(veciter = (iterInvert->second).begin(); veciter != (iterInvert->second).end(); veciter++)
        {
            double wordweight = ((veciter->second)*idf / sqrt(sumMap[veciter->first]));
            ofs << veciter->first << "\t" << veciter->second << "\t" << wordweight << "\t";
        }
        ofs << std::endl;
    }
    ofs.close();
    std::cout << "invertIndex over!" << std::endl;
}

void DealPage::removeDuplication(std::vector<Page> & pagevec)
{
    int indexi, indexj;
    for(indexi = 0; indexi != pagevec.size()-1; indexi++)
    {
        for(indexj = indexi + 1; indexj != pagevec.size(); indexj++)
        {            
            if(pagevec[indexi] == pagevec[indexj])
            {
                 Page tmp = pagevec[indexj];
                 //std::cout << "hello" << std::endl;
                 pagevec[indexj] = pagevec[pagevec.size() - 1];
                 pagevec[pagevec.size() - 1] = tmp;
                 pagevec.pop_back();
                 indexj--;
            }
        }
    }
    std::ofstream ofs(conf_.getMap()["my_newoffset"]);
    if(!ofs)
    {
        std::cout << "removeDuplication open file error" << std::endl;
        exit(EXIT_FAILURE);
    }
    int pageindex;
    for(pageindex = 0; pageindex != pagevec.size(); pageindex++)
    {
        std::cout << "pagevec.size():" << pagevec.size() << std::endl;
        ofs <<pagevec[pageindex].getdocid() << "\t" << offset_[pagevec[pageindex].getdocid()].first << "\t" << offset_[pagevec[pageindex].getdocid()].second << std::endl;
    }
    ofs.close();
    ofs.clear();
}

void DealPage::invertIndex(std::vector<Page> &pagevec,std::map<std::string, std::vector<std::pair<int, int> > > &indexmap)
{
    int index;
    for(index = 0; index !=pagevec.size(); index++)
    {
        std::map<std::string, int>::iterator iter;
        for(iter= (pagevec[index].wordfrequency_).begin(); iter != (pagevec[index].wordfrequency_).end(); iter++)
        {
            indexmap[iter->first].push_back(std::make_pair(pagevec[index].getdocid(), iter->second));           
        }


    }
}

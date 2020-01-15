#include "Page.h"
#include "MyConf.h"
#include "MySplit.h"
#include <string>
#include <queue>
#include <algorithm>

bool operator ==(const Page &lhs, const Page &rhs)
{
    std::vector<std::string>::const_iterator citerleft = lhs.topstrvec_.begin();
    int num = 0;
    for(; citerleft != lhs.topstrvec_.end(); citerleft++)
    {
        num += count(rhs.topstrvec_.begin(), rhs.topstrvec_.end(), *citerleft);
    }
    int veclen = lhs.topstrvec_.size() < rhs.topstrvec_.size() ? lhs.topstrvec_.size() : rhs.topstrvec_.size();
    if((double)num / veclen > 0.6)
    {
        return true;
    }
    else
    {
        return false;
    }
}
Page::Page(const std::string & docstr, const MyConf & myconf, MySplit & split)
{
    docsplit(docstr, myconf,split);
}

void Page::docsplit(const std::string & docstr,const MyConf & myconf, MySplit & split)
{
    //std::cout << "docstr:" << docstr << std::endl;
    //std::map<std::string, std::string> & fileparhmap = myconf.getMap();
    std::string head_docid = "<docid>";
    std::string tail_docid = "</docid>";
    //std::string head_docurl = "<docurl>";
    //std::string tail_docurl = "</docurl>";
    std::string head_doctitle = "<doctitle>\n";
    std::string tail_doctitle = "\r\n</doctitle>";
    std::string head_doccontent = "<doccontent>\n";
    std::string tail_doccontent = "\n</doccontent>";
    //提取docid
    std::string::size_type nbpos = docstr.find(head_docid);
    std::string::size_type nepos = docstr.find(tail_docid,nbpos);
    docid_ = docstr.substr(nbpos + head_docid.size(), nepos - nbpos - head_docid.size());
    //std::cout << "docid_:" << docid_ << std::endl;
    //提取doctitle
    nbpos = docstr.find(head_doctitle);
    nepos = docstr.find(tail_doctitle,nbpos);
    doctitle_ = docstr.substr(nbpos + head_doctitle.size(), nepos - nbpos - head_doctitle.size());
    //提取doccontent
    nbpos = docstr.find(head_doccontent);
    nepos = docstr.find(tail_doccontent,nbpos);
    doccontent_ = docstr.substr(nbpos + head_doccontent.size(), nepos - nbpos - head_doccontent.size());

    //用分词类分词,去stoplist
    std::vector<std::string> wordVec;
    wordVec = split(doccontent_);
    //生成top10
    createtop10(wordVec);
}

void Page::createtop10(const std::vector<std::string> & wordVec)
{
    std::vector<std::string>::const_iterator citer;
    for(citer = wordVec.begin(); citer != wordVec.end(); citer++)
    {
        wordfrequency_[*citer]++;
    }
    std::priority_queue<std::pair<std::string, int>, std::vector<std::pair<std::string, int> >, MyCompare> wordqueue(wordfrequency_.begin(), wordfrequency_.end());
    while(!wordqueue.empty())
    {
        topstrvec_.push_back(wordqueue.top().first);
        wordqueue.pop();
        if(topstrvec_.size() == 10)
        {
            break;
        }
    }
}

std::string Page::summary(std::vector<std::string> & queryvec)
{
    std::vector<std::string> summaryvec;
    std::istringstream iss(doccontent_);
    std::string line;
    while(iss >> line)
    {
        for(auto iter = queryvec.begin(); iter != queryvec.end(); iter++)
        {
            if(line.find(*iter) != std::string::npos)
            {
                summaryvec.push_back(line);
                break;
            }            
        }

        if(summaryvec.size() >= 5)
        {
            break;
        }
    }

    std::string summary;
    auto iter = summaryvec.begin();
    for(; iter != summaryvec.end() - 1; iter++)
    {
        summary.append(*iter).append("\n");
    }
    summary.append(*iter).append("...");;

    return summary;
}






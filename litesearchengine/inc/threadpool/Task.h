#ifndef _TASK_H_
#define _TASK_H_

#include <string>
#include <queue>
#include <set>

class Cache;
class MyDict;

struct MyResult
{
    std::string word_;//待查词汇的候选词
    int ifreq_;//本候选词的词频
    int idist_;//本候选词和待查词汇的编辑距离
};

struct MyCompare
{
    bool operator()(const MyResult &lhs, const MyResult &rhs)
    {
        if(lhs.idist_ > rhs.idist_)
        {
            return true;
        }
        else if(lhs.idist_ == rhs.idist_ && lhs.ifreq_ > rhs.ifreq_)
        {
            return true;
        }
        else if(lhs.idist_ == rhs.idist_ && lhs.ifreq_ > rhs.ifreq_ && lhs.word_ > rhs.word_)
        {
            return true;
        }
        else
            return false;
    }
};

class Task
{
public:
    Task(const std::string &expr, int sockfd, MyDict &mydict);
    Task(const char *expr, int sockfd, MyDict &mydict);
    //从cache里查找是否命中，是则发送给客户端，否职责从新从词典查找，并放入cache
    void execute(Cache &cache);
private:
    //从词典中查询，需计算计算最小编辑距离
    void query_idx_table();
    void statistic(std::set<int> &iset);
    //计算两个词的编辑距离
    int distance(const std::string &rhs);
    void response(Cache & cache);

private:
    //待查词
    std::string expr_;
    //与客户端的连接句柄
    int sockfd_;
    MyDict &mydict_;
    std::priority_queue<MyResult, std::vector<MyResult>, MyCompare> que_res_;
};
#endif

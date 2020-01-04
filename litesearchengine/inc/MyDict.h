#ifndef _MYDICT_H_
#define _MYDICT_H_

#include <string>
#include <vector>
#include <set>
#include <utility>
#include <map>

class MyDict
{
public:
    //创建MyDict单例对象指针
    static MyDict * createInstance(const char *dictfilepath,const char *indexfilepath);
    //返回词典vector地引用
    std::vector<std::pair<std::string, int> > & get_dict();
    //返回索引map地引用
    std::map<std::string, std::set<int> > & get_index_table();

    void show_dict();
    void show_index_table();
private:
    //单例模式，构造函数私有化
    MyDict(const char *dictfilepath, const char *indexfilepath);
    void read_from(const char *filepath);
    void record_to_index(int idx);

private:
    //单例模式，静态对象指针
    static MyDict *pInstance_;
    //词语以及词频
    std::vector<std::pair<std::string, int> > dict_;
    //字符，以及出现该字符所在vector下标
    std::map<std::string, std::set<int> > index_table_;
};

#endif

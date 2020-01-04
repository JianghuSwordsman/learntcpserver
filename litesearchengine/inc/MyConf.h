#ifndef _MYCONF_H_
#define _MYCONF_H_

#include <string>
#include <sstream>
#include <map>

class MyConf
{
public:
    MyConf(std::string &);
    //读取配置文件到map
    bool init();

    std::map<std::string, std::string> & getMap()
    {
        return map_;
    }

    void show();
private:
    //配置文件路径
    std::string filepath_;
    //存储配置信息项和值
    std::map<std::string, std::string> map_;
};
#endif

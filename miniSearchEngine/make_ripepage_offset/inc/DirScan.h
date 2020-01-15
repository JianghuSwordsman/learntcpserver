#ifndef _DIRSCAN_H_
#define _DIRSCAN_H_

#include <vector>
#include <string>

//遍历目录类，该类递归扫描指定目录下的文件，将所有文件路径报错到容器
class DirScan
{
public:
    //传入保存文件路径的容器
    DirScan(std::vector<std::string> & vecstr);
    //重载函数调用运算符，传入待遍历的文件路径
    void operator()(const std::string & dirpath);

private:
    void traverse(const std::string & dirpath);
private:
    std::vector<std::string> & vecstr_;
};
#endif

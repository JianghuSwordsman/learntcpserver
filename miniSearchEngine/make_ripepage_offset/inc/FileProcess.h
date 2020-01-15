#ifndef _FILEPROCESS_H_
#define _FILEPROCESS_H_

#include <string>
#include <vector>
#include <map>
class FileProcess
{
public:
    //传入文件路径vec和“主题"的名称
    FileProcess(std::vector<std::string> & strvec, std::string & title);
    void operator()(const std::string & pagefilepath, const std::string & offsetfilepath);
private:
    //读取文档内容，格式化后写入page文本库，并文档编号、偏移量和长度写到偏移文本库中
    void createPageAndOffsetFile(const std::string & pagefilepath, const std::string & offsetfilepath);
    //读取文档内容，返回文档内容和标题
    void readFile(const std::string & filepath, std::string & contentstr, std::string &titlestr);
private:
    //保存文件路径
    std::vector<std::string> & strvec_;
    //保存提取标题
    std::string title_;
    //存储对应id网页的，开始偏移位置和长度
    std::map<int, std::pair<int, int> > offset_;
};
#endif

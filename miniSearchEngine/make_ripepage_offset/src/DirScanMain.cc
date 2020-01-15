#include "DirScan.h"
#include "FileProcess.h"
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <algorithm>

void show(std::vector<std::string>::value_type & val)
{
    std::cout << val << std::endl;
}

int main(int argc, char *argv[])
{
    //读取“标题”标志
    std::string title;
    std::ifstream ifs("title.txt");
    if(!ifs)
    {
        std::cout << "ifstream error" << std::endl;
        exit(EXIT_FAILURE);
    }
    std::getline(ifs, title);
    std::cout << title << std::endl;

    //定义遍历文档资料目录类对象
    std::vector<std::string> strvec;
    DirScan dirscan(strvec);
    //扫描某目录下的文档资料
    std::string docpath = "/home/c++/miniSearchEngine/make_ripepage_offset/data";
    dirscan(docpath);

    //for_each(strvec.begin(), strvec.end(), show);

    //定义文件处理对象，生成page和offset库
    FileProcess fileprocess(strvec, title);
    std::string pagefilepath = "/home/c++/miniSearchEngine/make_ripepage_offset/bin/page.data";
    std::string offsetfilepath = "/home/c++/miniSearchEngine/make_ripepage_offset/bin/offset.data";
    fileprocess(pagefilepath, offsetfilepath);

   return 0; 
}

#include "DirScan.h"
#include "Log.h"
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>

DirScan::DirScan(std::vector<std::string> & vecstr)
    :vecstr_(vecstr)
{}

void DirScan::operator()(const std::string &dirpath)
{
    traverse(dirpath);
}

void DirScan::traverse(const std::string & dirpath)
{
#if 0    
    std::ofstream ofs(dirpath);
    if(!ofs)
    {
        LOG_ERROR(std::string("DirScan.cc traverse open ofstream error."));
        exit(EXIT_FAILURE);
    }
#endif
    struct dirent *pdirent;
    struct stat filestat;
    
    //std::cout << dirpath << std::endl;
    //打开指定目录，返回指针用于获取此目录胡信息
    DIR *pdir = opendir(dirpath.c_str());
    if(!pdir)
    {
        LOG_ERROR(std::string("traverse opendir error."));
        exit(EXIT_FAILURE);
    }
    //进入指定目录
    chdir(dirpath.c_str());

    //循环便利目录下所有文件
    while((pdirent = readdir(pdir)) != NULL)
    {
        //std::cout << pdirent->d_name << std::endl;
        //获取目录属性
        stat(pdirent->d_name, &filestat);
        //判断是不是目录，是./..则跳过，是目录则递归执行本函数
        //是文件则把路径放到vecstr_
        if(S_ISDIR(filestat.st_mode))
        {
            if(strncmp(pdirent->d_name, ".", 1) == 0 || strncmp(pdirent->d_name, "..", 2) == 0)
            {
                continue;
            }else{
                traverse(pdirent->d_name);
            }
        }else{
        
            std::string filename = "";
            filename.append(getcwd(NULL,0));
            filename.append("/");
            filename.append(pdirent->d_name);
            //std::cout << filename << std::endl;

            vecstr_.push_back(filename);
        }
    }
    chdir("..");
    closedir(pdir);
}//end traverse



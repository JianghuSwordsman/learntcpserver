#include "FileProcess.h"
#include "Log.h"
#include <fstream>
#include <stdlib.h>
#include <sstream>

FileProcess::FileProcess(std::vector<std::string> & strvec, std::string & title)
    :strvec_(strvec),
    title_(title)
{}

void FileProcess::operator()(const std::string &pagefilepath, const std::string & offsetfilepath)
{
    createPageAndOffsetFile(pagefilepath, offsetfilepath);
}

void FileProcess::createPageAndOffsetFile(const std::string & pagefilepath, const std::string & offsetfilepath)
{
    std::ofstream pageofs(pagefilepath.c_str());
    std::ofstream offsetofs(offsetfilepath.c_str());
    if(!pageofs || !offsetofs)
    {
        LOG_ERROR(std::string("FileProcess.cc createPageAndOffsetFile open ofstream error."));
        exit(EXIT_FAILURE);
    }
    
    std::string titlestr;
    std::string textstr;
    std::string contentstr;
    std::string urlstr;
    std::size_t docid = 0;

    for(auto vecpathstridx : strvec_)
    {
        titlestr.clear();
        textstr.clear();
        contentstr.clear();
        urlstr.clear();
        //读取文件内容，返回内容和标题
        readFile(vecpathstridx, contentstr, titlestr);

        docid += 1;
        std::ostringstream oss;
        oss << docid;
        std::string docidstr = oss.str();
        urlstr = vecpathstridx;
        //将原文档格式化并存储到page
        textstr.append("<doc>\n<docid>").append(docidstr).append("</docid>\n<docurl>").append(urlstr).append("</docurl>\n<doctitle>\n").append(titlestr).append("\n</doctitle>\n<doccontent>\n").append(contentstr).append("\n</doccontent>\n</doc>\n");
        //计算文档在page中的偏移量
        std::ofstream::pos_type pos = pageofs.tellp();
        std::string::size_type textsize = textstr.size();
        //把格式化后的文档写入page文档库
        pageofs << textstr;

        //std::string posstr = std::to_string(pos);
        //std::string textsizestr = std::to_string(textsize);
        //偏移信息写入偏移文件中
        offsetofs << docidstr << "\t" << pos << "\t" << textsize << std::endl;
    }
    pageofs.close();
    offsetofs.close();
}

void FileProcess::readFile(const std::string & filepath, std::string & contentstr, std::string & titlestr)
{
    //std::ifstream ifs(filepath.c_str());
    std::ifstream ifs(filepath);
    if(!ifs)
    {
        LOG_ERROR(std::string("FileProcess.cc readFile  open ifstream error."));
        exit(EXIT_FAILURE);
    }
  
    
    std::streamsize size = ifs.seekg(0,std::ios::end).tellg();
    contentstr.resize(size);
    std::cout << "contentstr.size():" << contentstr.size() << "contentstr.capacity():" << contentstr.capacity() << "size:" << size << std::endl;
    ifs.seekg(0).read(&contentstr[0], size);
    

    //文件光标重新回到开头
    ifs.seekg(0);

    std::string line;
    size_t count = 0;
    bool flag = false;
    //读取前11行，如果有标题，则返回对应的标题，如果没有则将12作为标题，
    //如果没有11行，则把第一行作为标题
    while(count <= 10 && std::getline(ifs, line))
    {
        std::string::size_type pos = line.find(title_.c_str(),0);
        if(pos != std::string::npos)
        {
            pos += title_.size();
            titlestr = line.substr(pos);
            flag = true;
            break;
        }
        count ++;        
    }
    if(flag == false && count < 11)
    {
        ifs.seekg(0);
        std::getline(ifs, line);
    }
    else if(flag == false && count == 11)
    {
        std::getline(ifs, line);
    }
    if(titlestr[titlestr.size() - 1] == '\n')
        titlestr[titlestr.size() - 1] = '\0';

    ifs.close();
}


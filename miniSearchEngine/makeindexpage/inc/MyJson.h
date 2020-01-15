#ifndef _MYJSON_H_
#define _MYJSON_H_

#include <string>
#include <iostream>
#include <vector>
#include <utility>
#include <stdio.h>
#include <ext/hash_map>
#include <string.h>
#include <json/json.h>
#include "Page.h"

class MyJson
{
public:
    //把查询的结果，封装json字符串
    std::string operator ()(std::vector<std::pair<int, std::vector<double> > > & resvec, __gnu_cxx::hash_map<int, Page> & pagemap, std::vector<std::string> &queryvec);
    //如果没有查询结果，固定返回字符
    std::string noresult();
};
#endif

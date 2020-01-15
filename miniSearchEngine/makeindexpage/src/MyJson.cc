#include "MyJson.h"

std::string MyJson::operator ()(std::vector<std::pair<int, std::vector<double> > > & resvec, __gnu_cxx::hash_map<int, Page> & pagemap, std::vector<std::string> &queryvec)
{
    Json::Value root;
    Json::Value arr;
    int residx;
    for(residx = 0; residx != resvec.size(); residx++)
    {
        std::string summary = pagemap[resvec[residx].first].summary(queryvec);
        std::string title = pagemap[resvec[residx].first].doctitle_;
        if(title.size() > 200)
            title = title.substr(0,200);
        Json::Value elem;
        elem["title"] = title;
        elem["summary"] = summary;
        arr.append(elem);
    }
    root["files"] = arr;
    Json::StyledWriter stlwriter;
    return stlwriter.write(root);
}

std::string MyJson::noresult()
{
    Json::Value root;
    Json::Value arr;
    Json::Value elem;
    elem["title"] = "no answer";
    elem["summary"] = "cann't find what you want, I am so sorry!";
    arr.append(elem);
    root["files"] = arr;
    Json::StyledWriter stlwriter;
    return stlwriter.write(root);
}

#include "MySplit.h"
#include "NLPIR.h"
#include "MyConf.h"
#include <stdlib.h>
#include <vector>
#include <iostream>

MySplit::MySplit(MyConf & conf)
   : conf_(conf)
{
    if(!NLPIR_Init("./NLPIR", UTF8_CODE))
    {
        std::cout << "NLPIR Init fail!" << std::endl;
        exit(EXIT_FAILURE);
    }
}

MySplit::~MySplit()
{
    NLPIR_Exit();
}

std::vector<std::string>  MySplit::operator()(const std::string &str)
{
    const char * dest = NLPIR_ParagraphProcess(str.c_str(), 0);
    std::istringstream iss(dest);
    std::vector<std::string> wordvec;
    std::string word;
    while(iss >> word)
    {
        if(!conf_.stop_.count(word) && word[0] != '\r')
        {
            wordvec.push_back(word);        
        }
    }
    return wordvec;
}


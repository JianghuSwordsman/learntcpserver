#ifndef _LOG_H_
#define _LOG_H_

#include <string>
#include <iostream>

inline void LOG_ERROR(const std::string &str)
{
    std::cout << __DATE__ << __FILE__ << "[" << __LINE__ << "]: " << str << std::endl;
}
#endif

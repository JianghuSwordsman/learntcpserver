#include "InetAddress.h"
#include "ThreadPool.h"
#include "Task.h"
#include "TcpServer.h"
#include "MyConf.h"
#include "MyDict.h"
#include "Cache.h"
#include "MyTimerThread.h"
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <map>

static ThreadPool *pThreadPool = NULL;

void onConnection(const TcpConnectionPtr & conn);
void onMessage(const TcpConnectionPtr & conn);
void onClose(const TcpConnectionPtr & conn);

int main()
{   //初始化配置信息
    std::string filepath = "/home/c++/litesearchengine/conf/my.conf";
    MyConf myconf(filepath);
    bool initFlag = myconf.init();
    if(initFlag)
    {
        myconf.show();
    }
    //初始化词典
    std::map<std::string, std::string> &myMap = myconf.getMap();
    std::string dictpath = myMap["my_dict"];
    std::string dictpath2 = myMap["my_cn_dict"];
    MyDict *pMydict = MyDict::createInstance(dictpath.c_str(), dictpath2.c_str());

    //初始化缓存
    std::string cachepath = myMap["my_cache"];
    Cache cache;
    cache.read_from_file(cachepath.c_str());

    //初始化线程池
    ThreadPool thp(50, 10, cache, (*pMydict));
    thp.start();
    pThreadPool = &thp;

    //初始化定时器
    MyTimerThread myTmThread(1, 100, std::bind(&ThreadPool::update, &thp));
    myTmThread.start();

    //获取服务器端的IP和PORT
    std::string strip = myMap["my_ip"];
    std::string strport = myMap["my_port"];

    int port = 0;
    std::stringstream ss(strport);
    ss >> port;

    //创建服务器并进行监听套接字
    InetAddress inetAddr(strip,port);
    TcpServer server(inetAddr);
    server.setConnectCallback(&onConnection);
    server.setMessageCallback(&onMessage);
    server.setCloseCallback(&onClose);
    server.start();

    return 0;
}

void onConnection(const TcpConnectionPtr & conn)
{
    printf("%s has connected.\n", conn->toString().c_str());
    conn->send("hello, welcome to chat server");
}

void onMessage(const TcpConnectionPtr & conn)
{
    std::string msg(conn->receive());
    std::string::size_type n = msg.find('\n');
    if(n != std::string::npos)
    {
        msg = msg.substr(0,n);
    }
    std::cout << "receive: " << msg << ",sizeof(msg) = " << msg.size() << std::endl;
    Task task(msg, conn->fd(), pThreadPool->mydict_);
    pThreadPool->addTask(task);   
}

void onClose(const TcpConnectionPtr & conn)
{
    printf("close connection: %s.\n",conn->toString().c_str());
}

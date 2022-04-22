#pragma once
#include<condition_variable>
#include"../Thread.h"
#include"../Pool/redisConnectionPool.h"
class MYSQL;
class dbSYN{
public:
    dbSYN(int _interval);
    ~dbSYN();
    void setConn();
    void startSyn();
    void synFuncOnce();
    void synFuncLast();
    void setExitLabel();
    bool getExitLabel();
private:
    bool exitLabel=false;
    redisContext* redisConn;
    MYSQL* mysqlConn;
    std::mutex mutex_;
    std::condition_variable cond_;
    Thread thread_;
    int interval;
    int voteCount[2][4];
};
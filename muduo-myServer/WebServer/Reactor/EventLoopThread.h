#pragma once
<<<<<<< HEAD
=======
#include<memory>
>>>>>>> 6d98924 (first common)
#include"../varia.h"
#include"../Thread.h"
#include"../syncTools/Mutex.h"
#include"../syncTools/Condition.h"
class EventLoop;
class Mysql_pool;
<<<<<<< HEAD
class EventLoopThread:public noncopyable{
public:
    EventLoopThread(Mysql_pool* mysqlPool,int order,std::string name=std::string());
=======
class CachePool;
class EventLoopThread:public noncopyable{
public:
    EventLoopThread(Mysql_pool* mysqlPool,CachePool* redisPool,int order,std::string name=std::string());
>>>>>>> 6d98924 (first common)
    ~EventLoopThread();
    EventLoop* startLoop();
private:
    void threadFunc();
    EventLoop* loop_;
    Thread thread_;
    int order_;
    MutexLock mutex_;
    Condition cond_;
    Mysql_pool* mysqlPool_;
<<<<<<< HEAD
=======
    CachePool* redisPool_;
>>>>>>> 6d98924 (first common)
};
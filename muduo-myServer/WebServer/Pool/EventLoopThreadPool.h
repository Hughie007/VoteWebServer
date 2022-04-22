#pragma once
#include<vector>
#include<memory>
#include"../varia.h"
class Mysql_pool;
<<<<<<< HEAD
=======
class CachePool;
>>>>>>> 6d98924 (first common)
class EventLoop;
class EventLoopThread;
class EventLoopThreadPool:public noncopyable{
private:
    EventLoop* baseloop_;
    int numThreads_;
    std::string name_;
    std::vector<std::unique_ptr<EventLoopThread>> threads_;
    std::vector<EventLoop*> loops_;
    int next_;
public:
    EventLoopThreadPool(EventLoop* baseloop);
    ~EventLoopThreadPool();
    void setThreadNum(int nums){
        numThreads_=nums;
    }
<<<<<<< HEAD
    void start(Mysql_pool* mysqlPool);
=======
    void start(Mysql_pool* mysqlPool,CachePool* redisPool);
>>>>>>> 6d98924 (first common)
    EventLoop* getNextLoop();
};
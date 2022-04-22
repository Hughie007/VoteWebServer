#pragma once
#include<memory>
#include<queue>
#include <sys/time.h>
#include "../varia.h"
class TcpConn;
class TimeNode{
public:
    TimeNode(std::shared_ptr<TcpConn> spTcpConn,int timeout);
    ~TimeNode();
    size_t getExpiredTime() const;
    void update(int timeout);
    bool isDelete() const;
private:
    bool delete_;
    size_t expiredTime_;
    std::shared_ptr<TcpConn> spTcpConn_;
};
struct TimeCmp{
    bool operator()(std::shared_ptr<TimeNode>& a,std::shared_ptr<TimeNode>& b) const{
        return a->getExpiredTime()>b->getExpiredTime();
    }
};
class TimeQue{
public:
    TimeQue();
    ~TimeQue();
    void handleTimeQueue();
private:
    typedef std::shared_ptr<TimeNode> sp_TimeNode;
    std::priority_queue<sp_TimeNode,std::deque<sp_TimeNode>,TimeCmp> TimeQueueRecord_;
};
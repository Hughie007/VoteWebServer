#include "TimeQueue.h"

TimeNode::TimeNode(std::shared_ptr<TcpConn> spTcpConn,int timeout)
    :spTcpConn_(spTcpConn),
     delete_(false){
         struct timeval tv;
         gettimeofday(&tv,NULL);
         expiredTime_=((tv.tv_sec%10000)*1000+tv.tv_usec*1000)+timeout;
     }
TimeNode::~TimeNode(){}
size_t TimeNode::getExpiredTime() const{
    return expiredTime_;
}

bool TimeNode::isDelete() const{
    return delete_;
}
void TimeNode::update(int timeout){
    struct timeval tv;
    gettimeofday(&tv,NULL);
    expiredTime_=((tv.tv_sec%10000)*1000+tv.tv_usec*1000)+timeout;
}










TimeQue::TimeQue(){}
TimeQue::~TimeQue(){}
void TimeQue::handleTimeQueue(){
    struct timeval tv;
    gettimeofday(&tv,NULL);
    size_t now=(tv.tv_sec%10000)*1000+tv.tv_usec*1000;
    while(!TimeQueueRecord_.empty()){
        std::shared_ptr<TimeNode> p=TimeQueueRecord_.top();
        if(p->getExpiredTime()<now){
            TimeQueueRecord_.pop();
        }
        else{
            break;
        }
    }
}
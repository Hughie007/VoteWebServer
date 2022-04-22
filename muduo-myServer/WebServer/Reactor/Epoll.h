#pragma once
#include<memory>
#include<sys/epoll.h>
#include<vector>
class Channel;
typedef std::shared_ptr<Channel> sp_Chan;
class Epoll{
public:
    Epoll();
    ~Epoll();
    void epoll_add(sp_Chan Channel,int timeout=0);
    void epoll_mod(sp_Chan Channel);
    void epoll_del(sp_Chan Channel);
    std::vector<sp_Chan> poll();
    std::vector<sp_Chan> getRequestChan(int eventsNum);
private:
    static const int maxFdNum=100000;
    int epollFd_;
    std::shared_ptr<Channel> fd2Chan_[maxFdNum];     //记录已注册fd对应的channel，监听到响应fd时取出对应channel处理事件
    std::vector<epoll_event> events_;
};
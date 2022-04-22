#pragma once
#include<functional>
#include<memory>
#include"../TcpConnection/Socket.h"
#include"../varia.h"
class EventLoop;
class InetAddress;
class Channel;
class TimeStamp;
class Acceptor:public noncopyable{
public:
    typedef std::function<void(int sockfd,const InetAddress&)> NewConnectionCallback;
    Acceptor(EventLoop* loop,const InetAddress& listenAddr,bool reusePort);
    ~Acceptor();
    void setNewConnectCallback(const NewConnectionCallback& cb){
        newConnCallback_=cb;
    }
    void listen();
private:
    EventLoop* loop_;
    void handleRead(TimeStamp ts);
    Socket acceptorFd_;
    std::shared_ptr<Channel> acceptorChannel_;
    bool listening_;
    NewConnectionCallback newConnCallback_; 
};
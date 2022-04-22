#pragma once
#include<memory>
#include<functional>
#include"../InetAddress.h"
#include"stateAnalyse.h"
#include"Buffer.h"
#include"../Callback.h"

class Socket;
class Channel;
class EventLoop;
class TcpConn:public noncopyable,public std::enable_shared_from_this<TcpConn>{
public:
    TcpConn(EventLoop* loop,std::string connName,int connfd,const InetAddress& localAddr,const InetAddress& peerAddr);
    ~TcpConn();
    void setConnectionCallback(const ConnectionCallback& cb){connectionCallback_=cb;}
    void setMessageCallback(const MessageCallback& cb){messageCallback_=cb;}
    void setCloseCallback(const CloseCallback& cb){closeCallback_=cb;}
    void connectEstablished();
    void writeEstablished();
    void connectDestroyed();
    bool connected() const;
    void setTcpNoDelay(bool on);                       //禁用Nagle算法，避免连续发包出现延迟
    const InetAddress& getInetAddress() const{
        return peerAddr_;
    }
    void send(const std::string& message);
    void sendInLoop(const std::string& message);
    void shutDown();
    void shutDownInLoop();
    std::string name() const{return name_;}
    EventLoop* getEventLoop() const{return loop_;}
    stateAnalyse& getStateRecord_(){return stateRecord_;}
    void setWeakEntryPtr(weakEntryPtr wp_Entry){wp_Entry_=wp_Entry;};
    weakEntryPtr getWeakEntryPtr(){return wp_Entry_;};
    void addTimer();
    void handleClose();                           //设置为公有函数以便定时器操作
private:
    enum StateE{ kConnecting,kConnected,kDisconnected,kDisconnecting};
    void setState(StateE state){state_=state;}
    void handleRead(TimeStamp);
    void handleWrite();
    std::unique_ptr<Socket> connfd_;
    std::shared_ptr<Channel> channel_;
    EventLoop *loop_;
    std::string name_;
    StateE state_;
    InetAddress localAddr_;
    InetAddress peerAddr_;
    ConnectionCallback connectionCallback_;
    MessageCallback messageCallback_;
    CloseCallback closeCallback_;
    Buffer inputBuffer_;
    Buffer outputBuffer_;
    stateAnalyse stateRecord_;
    weakEntryPtr wp_Entry_;
};
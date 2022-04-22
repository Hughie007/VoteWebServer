#pragma once
#include<functional>
#include<memory>
#include<assert.h>
#include<poll.h>
#include"../varia.h"
class EventLoop;
class TcpConn;
class TimeStamp;

class Channel:public noncopyable,public std::enable_shared_from_this<Channel>{
public:
    typedef std::function<void()> EventCallback;
    typedef std::function<void(TimeStamp)> ReadCallback;
    Channel(EventLoop* loop,int fd);               
    ~Channel();
    void setReadCallback(ReadCallback cb);
    void setWriteCallback(EventCallback cb);
    void setCloseCallback(EventCallback cb);
    int fd() const;
    int events() const;
    void handleEvent(TimeStamp ts);
    bool EqualAndUpdateEvent();
    void enableReading();
    void enableWriting();
    void disableWriting();
    void disableAll();
    std::shared_ptr<TcpConn> getHolder();
    EventLoop* ownerLoop() const;
    void setRevents(uint32_t event);
    void setEvents(uint32_t event);
    bool isWriting() const;
private:
    void update();
    static const int ReadEvents;
    static const int WriteEvents;
    static const int kNoneEvents;
    bool eventHandling_;
    int fd_;        
    bool addToLoop;                               //并不拥有fd
    EventLoop *loop_;
    int events_;
    int revents_;
    std::weak_ptr<TcpConn> wp_TcpConn;         
    ReadCallback readHander_;
    EventCallback writeHander_;
    EventCallback closeHander_;

};
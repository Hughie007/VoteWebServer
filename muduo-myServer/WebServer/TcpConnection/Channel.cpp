#include "Channel.h"
#include"../TimeManager/TimeStamp.h"
#include"../Log/logging.h"
#include"../Reactor/EventLoop.h"
const int Channel::ReadEvents=POLLIN|POLLPRI;
const int Channel::WriteEvents=POLLOUT;
const int Channel::kNoneEvents=0;
Channel::Channel(EventLoop *loop,int fd)
    :loop_(loop),
     fd_(fd),
     addToLoop(false),
     events_(0),
     revents_(0),
     eventHandling_(false)
     {}
Channel::~Channel(){
    assert(!eventHandling_);     //断言事件处理期间不会析构
}
void Channel::setReadCallback(ReadCallback cb){
    readHander_=std::move(cb);
}
void Channel::setWriteCallback(EventCallback cb){
    writeHander_=std::move(cb);
}
void Channel::setCloseCallback(EventCallback cb){
    closeHander_=std::move(cb);
}
int Channel::fd() const{
    return fd_;
}
int Channel::events() const{
    return events_;
}
void Channel::handleEvent(TimeStamp ts){
    eventHandling_=true;
    if(revents_&(POLLIN|POLLPRI)){
        if(readHander_)readHander_(ts);
    }
    if(revents_&POLLOUT){
        if(writeHander_)writeHander_();
    }
    if((revents_ & POLLHUP)&&!(revents_ & POLLIN)){
        LOG<<"Channel::handle_event() POLLHUP";
        if(closeHander_)closeHander_();
    }
    eventHandling_=false;
}
bool Channel::EqualAndUpdateEvent(){
    bool ret= (revents_==events_);
    revents_=events_;
    return ret;
}
std::shared_ptr<TcpConn> Channel::getHolder(){
    std::shared_ptr<TcpConn> ret(wp_TcpConn.lock());
    return ret;
}
void Channel::enableReading(){
    events_|=ReadEvents;
    update();
    addToLoop=true;
}
void Channel::enableWriting(){
    events_|=WriteEvents;
    update();
    addToLoop==true;
}
void Channel::disableWriting(){
    events_&=~WriteEvents;
    update();
}
void Channel::disableAll(){
    events_=kNoneEvents;update();
}
void Channel::update(){
    if(!addToLoop){
        loop_->addToPoller(shared_from_this());
    }
    else{
        loop_->updatePoller(shared_from_this());
    }
}
EventLoop* Channel::ownerLoop() const{
    return loop_;
}
void Channel::setRevents(uint32_t event){
    revents_=event;
}
void Channel::setEvents(uint32_t event){
    events_=event;
}
bool Channel::isWriting() const{
    return events_ & WriteEvents;
}
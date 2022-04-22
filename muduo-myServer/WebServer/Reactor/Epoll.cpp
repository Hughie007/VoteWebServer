#include "Epoll.h"
#include"../TcpConnection/Channel.h"
const int EPOLLWAIT_TIME=10000;
Epoll::Epoll()
    :epollFd_(epoll_create1(EPOLL_CLOEXEC)),   //创建的epfd会设置FD_CLOEXEC, 它是fd的一个标识说明, 用来设置文件close-on-exec状态的
     events_(4096)
    {
    }
Epoll::~Epoll(){  
}
void Epoll::epoll_add(sp_Chan spChan,int timeout){
    int fd=spChan->fd();
    struct epoll_event event;
    event.data.fd=fd;
    event.events=spChan->events();
    spChan->EqualAndUpdateEvent();
    fd2Chan_[fd] = spChan;
    if(epoll_ctl(epollFd_,EPOLL_CTL_ADD,fd,&event)<0){
        perror("epoll_add error");
        fd2Chan_[fd].reset();
    }
}
void Epoll::epoll_mod(sp_Chan spChan){
    int fd=spChan->fd();
    if(!spChan->EqualAndUpdateEvent()){
        struct epoll_event event;
        event.data.fd=fd;
        event.events=spChan->events();
        if(epoll_ctl(epollFd_,EPOLL_CTL_MOD,fd,&event)<0){
            perror("epoll_mod error");
            fd2Chan_[fd].reset();
        }
    }
}
void Epoll::epoll_del(sp_Chan spChan){
    int fd=spChan->fd();
    struct epoll_event event;
        event.data.fd=fd;
        event.events=spChan->events();
    if(epoll_ctl(epollFd_,EPOLL_CTL_DEL,fd,&event)<0){
        perror("epoll_del error");
    }
    fd2Chan_[fd].reset();
}
std::vector<sp_Chan> Epoll::poll(){
    while(true){
        int events_num=epoll_wait(epollFd_,&(*events_.begin()),events_.size(),-1);
        if(events_num<0)perror("epoll wait error");
        std::vector<sp_Chan> req_chan=getRequestChan(events_num);
        if(req_chan.size()>0)return req_chan;
    }
} 
std::vector<sp_Chan> Epoll::getRequestChan(int eventsNum){
    std::vector<sp_Chan> req_data;
    for(int i=0;i<eventsNum;i++){
        int fd=events_[i].data.fd;
        sp_Chan spChan=fd2Chan_[fd];
        if(spChan){
            spChan->setRevents(events_[i].events);
            //spChan->setEvents(0);                    //重置event事件
            req_data.push_back(spChan);
        }
        else{
            //Log..
        }
    }
    return req_data;
}
#include"EventLoopThreadPool.h"
#include"../Reactor/EventLoop.h"
#include"../Reactor/EventLoopThread.h"
EventLoopThreadPool::EventLoopThreadPool(EventLoop* loop)
    :baseloop_(loop),
     numThreads_(8),
     next_(0),
     name_("EventLoopThreadLoop")
     {}
EventLoopThreadPool::~EventLoopThreadPool()
    {}
<<<<<<< HEAD
void EventLoopThreadPool::start(Mysql_pool* mysqlPool){
=======
void EventLoopThreadPool::start(Mysql_pool* mysqlPool,CachePool* redisPool){
>>>>>>> 6d98924 (first common)
    baseloop_->assertInLoopThread();
    char name[]="EventLoopThread";
    for(int i=0;i<numThreads_;i++){
        char buf[32];
        snprintf(buf,sizeof buf,"%s%d",name,i);
<<<<<<< HEAD
        EventLoopThread* elt=new EventLoopThread(mysqlPool,i,buf);                    //buf是线程名字，mysqlPool是数据库连接池指针
=======
        EventLoopThread* elt=new EventLoopThread(mysqlPool,redisPool,i,buf);                    //buf是线程名字，mysqlPool是数据库连接池指针
>>>>>>> 6d98924 (first common)
        threads_.push_back(std::unique_ptr<EventLoopThread>(elt));
        loops_.push_back(elt->startLoop());
    }
}
EventLoop* EventLoopThreadPool::getNextLoop(){
    baseloop_->assertInLoopThread();
    EventLoop* loop=baseloop_;
    if(!loops_.empty()){
        loop=loops_[next_];
        next_++;
        if(next_>=loops_.size()){
            next_=0;
        }
    }
    return loop;
}
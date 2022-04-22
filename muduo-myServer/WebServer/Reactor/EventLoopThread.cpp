#include"EventLoopThread.h"
#include"EventLoop.h"
<<<<<<< HEAD
EventLoopThread::EventLoopThread(Mysql_pool* mysqlPool,int order,std::string name)
    :loop_(NULL),
     thread_(std::bind(&EventLoopThread::threadFunc,this),name),
     mutex_(),
     cond_(mutex_),
     mysqlPool_(mysqlPool),
     order_(order)
     {}
=======
#include"../Pool/redisConnectionPool.h"
EventLoopThread::EventLoopThread(Mysql_pool* mysqlPool,CachePool* redisPool,int order,std::string name)
    :loop_(NULL),
    thread_(std::bind(&EventLoopThread::threadFunc,this),name),
    mutex_(),
    cond_(mutex_),
    mysqlPool_(mysqlPool),
    redisPool_(redisPool),
    order_(order){
    }
>>>>>>> 6d98924 (first common)
EventLoopThread::~EventLoopThread(){
    if(loop_!=NULL){
        loop_->quit();
        thread_.join();
    }
}
EventLoop* EventLoopThread::startLoop(){
    assert(!thread_.started());
    thread_.start();
    {
        MutexLockGuard lock(mutex_);
        while(loop_==NULL)cond_.wait();           //待threadFunc跑起来，loop创建好了后再返回startLoop()
    }
    return loop_;
}
void EventLoopThread::threadFunc(){
    EventLoop loop(order_);
    loop.setMysqlPool(mysqlPool_);
<<<<<<< HEAD
    loop.UpdateVoteSituation();
=======
    loop.setRedisPool(redisPool_);
    loop.UpdateVoteSituation();
    loop.InitVoteSituation();
    loop.updateHtml();
>>>>>>> 6d98924 (first common)
    {
        MutexLockGuard lock(mutex_);
        loop_=&loop;
        cond_.notify();                            //通知主线程已经创建了Loop()
    }
    loop.loop();
    MutexLockGuard lock(mutex_);
    loop_=NULL;
}
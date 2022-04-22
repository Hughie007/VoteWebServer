#include"Thread.h"
#include"CurrentThread.h"
struct ThreadData{
    typedef Thread::ThreadFunc ThreadFunc;
    ThreadFunc func_;
    std::string name_;
    pid_t* tid_;
    CountDownLatch* latch_; 
    ThreadData(const ThreadFunc& func,const std::string& name,pid_t* tid,CountDownLatch* latch)
        :func_(func),name_(name),tid_(tid),latch_(latch){}
    void runInThread(){
        *tid_=CurrentThread::tid();       
        tid_=NULL;
        latch_->countDown();              //Thread的latch_
        latch_=NULL;
        func_();
    }
};
void * startThread(void* obj){
    ThreadData* data=static_cast<ThreadData*>(obj);
    data->runInThread();
    delete data;
    return NULL;
}
Thread::Thread(const ThreadFunc& func,const std::string& n)
    :started_(false),
     joined_(false),
     pthreadId_(0),
     tid_(0),
     func_(func),
     name_(n),
     latch_(1){
         setDefaultName();
     }
Thread::~Thread(){
    if(started_&&!joined_)pthread_detach(pthreadId_);
}
void Thread::setDefaultName(){
    if(name_.empty()){
        name_="Thread";
    }
}
void Thread::start(){
    assert(!started_);
    started_=true;
    ThreadData* data = new ThreadData(func_,name_,&tid_,&latch_);
    if(pthread_create(&pthreadId_,NULL,&startThread,data)){
        started_=false;
        delete data;
    }
    else{
        latch_.wait();        //为了保证func执行后，start()才返回
    }
}
bool Thread::started() const{
    return started_;
}
int Thread::join(){
    joined_=true;
    return pthread_join(pthreadId_,NULL);       //成功返回0,失败返回错误号
}
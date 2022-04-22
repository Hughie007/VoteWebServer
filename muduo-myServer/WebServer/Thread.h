
#pragma once
#include<string>
#include<functional>
#include<sys/unistd.h>
#include<sys/syscall.h>
#include"varia.h"
#include"./syncTools/CountDownLatch.h"


class Thread:public noncopyable{
public:
    typedef std::function<void()> ThreadFunc;
    explicit Thread(const ThreadFunc&,const std::string& name=std::string());
    ~Thread();
    void start();
    bool started() const;
    int join();
    pid_t const tid(){return tid_;};
    const std::string& name() const{return name_;}
private:
    void setDefaultName();
    bool started_;
    bool joined_;
    pthread_t pthreadId_;
    pid_t tid_;
    ThreadFunc func_;
    std::string name_;
    CountDownLatch latch_;
};
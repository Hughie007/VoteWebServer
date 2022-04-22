#pragma once
#include"Mutex.h"
#include "../varia.h"
class MutexLock;
class Condition:public noncopyable{
public:
    explicit Condition(MutexLock& mutex);
    ~Condition();
    void wait();
    void notify();
    void notifyall();
    bool waitForSeconds(int seconds);
private:
    MutexLock& mutex_;
    pthread_cond_t cond_;
};
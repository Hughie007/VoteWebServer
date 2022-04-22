#pragma once
#include"Mutex.h"
#include"Condition.h"
class CountDownLatch{
public:
    explicit CountDownLatch(int count);
    ~CountDownLatch(){};
    void wait();
    void countDown();
private:
    mutable MutexLock mutex_;
    Condition condition_;
    int count_;
};
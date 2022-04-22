#pragma once
#include<pthread.h>
#include<assert.h>
#include<semaphore.h>
#include"../varia.h"



class MutexLock: public noncopyable{
public:
    MutexLock();
    ~MutexLock();
    bool IsLockByThisThread();
    void assertLocked();
    void lock();
    void unlock();
    pthread_mutex_t* getPthreadMutex();
private:
    pthread_mutex_t mutex_;
    pid_t holder_;
};
class MutexLockGuard{
public:
    explicit MutexLockGuard(MutexLock& mutex);
    ~MutexLockGuard();
private:
    MutexLock& mutex_;
};
class sem{
private:
    sem_t sem_;
public:
    sem(){
        if(sem_init(&sem_,0,0)!=0){
            throw std::exception();
        }
    }
    sem(int num){
        if(sem_init(&sem_,0,num)!=0){
            throw std::exception();
        }
    }
    ~sem(){
        sem_destroy(&sem_);
    }
    bool wait(){
        return sem_wait(&sem_)==0;
    }
    bool post(){
        return sem_post(&sem_)==0;
    }
};


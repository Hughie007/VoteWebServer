#include "Mutex.h"
#include"../CurrentThread.h"
MutexLock::MutexLock():holder_(0){
    pthread_mutex_init(&mutex_,NULL);
}
MutexLock::~MutexLock(){
    holder_=0;
    pthread_mutex_destroy(&mutex_);
}
bool MutexLock::IsLockByThisThread(){
    return holder_==CurrentThread::tid();
}
void MutexLock::lock(){
    pthread_mutex_lock(&mutex_);
    holder_=CurrentThread::tid();
}
void MutexLock::unlock(){
    holder_=0;
    pthread_mutex_unlock(&mutex_);
}
pthread_mutex_t* MutexLock::getPthreadMutex(){
    return &mutex_;
}
void MutexLock::assertLocked(){
    assert(IsLockByThisThread());
}
MutexLockGuard::MutexLockGuard(MutexLock& mutex):mutex_(mutex){
    mutex_.lock();
}
MutexLockGuard::~MutexLockGuard(){
    mutex_.unlock();
}

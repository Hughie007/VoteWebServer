#include "LogFile.h"
#include"../syncTools/Mutex.h"
#include"FileUtil.h"
LogFile::LogFile(const std::string basename,int flushEveryN)
    :basename_(basename),
     flushEveryN_(flushEveryN),
     count_(0),
     mutex_(new MutexLock),
     file_(new AppendFile(basename)){
     }
LogFile::~LogFile(){flush();}
void LogFile::append(const char* information,size_t len){
<<<<<<< HEAD
    MutexLockGuard lock(*mutex_);                      
    append_unlocked(information,len);
}
void LogFile::flush(){
    MutexLockGuard lock(*mutex_);                      
=======
    MutexLockGuard lock(*mutex_);                      //只有一个线程对file和count进行操作，为什么要加锁
    append_unlocked(information,len);
}
void LogFile::flush(){
    MutexLockGuard lock(*mutex_);                      //只有一个线程对file_进行操作，为什么要加锁
>>>>>>> 6d98924 (first common)
    file_->flush();
}
void LogFile::append_unlocked(const char* information,size_t len){
    file_->append(information,len);
    ++count_;
    if(count_>flushEveryN_){
        count_=0;
        file_->flush();
    }
}
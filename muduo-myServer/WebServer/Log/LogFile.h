#pragma once
#include<memory>
#include "../varia.h"
class MutexLock;
class AppendFile;
class LogFile:public noncopyable{
public: 
    LogFile(const std::string basename,int flushEveryN=1024);
    ~LogFile();
    void append(const char* information,size_t len);
    void flush();
private:
    void append_unlocked(const char* information,size_t len);
    const std::string basename_;
    const int flushEveryN_;
    int count_;
    std::unique_ptr<MutexLock> mutex_;
    std::unique_ptr<AppendFile> file_;
};
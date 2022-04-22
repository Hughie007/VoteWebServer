#pragma once
#include<string>
#include "../varia.h"
class AppendFile:public noncopyable{
public:
    explicit AppendFile(std::string filename);
    ~AppendFile();
    void append(const char* information,const size_t len);
    void flush();
private:
    size_t write(const char* information,size_t len);
    FILE *fp_;
<<<<<<< HEAD
    char buffer_[64*1024];                 
=======
    char buffer_[64*1024];                 //为什么是64*1024
>>>>>>> 6d98924 (first common)
};
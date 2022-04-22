#pragma once
#include<memory>
#include<string>
#include"LogStream.h"
class Logger{
public:
    Logger(const char* filename,int line);
    ~Logger();
    LogStream& stream();
    static void setLogFileName(std::string str);
    static std::string getLogFileName();
private:
    class LogStreamAppend{                                //封装了文件，行号的LogStream
    public:
        LogStreamAppend(const char* filename,int line);
        ~LogStreamAppend();
        void formatTime();
        LogStream stream_;
        std::string basename_;
        int line_;
    };
    LogStreamAppend LSA;
    static std::string LogFileName_; 
};
#define LOG Logger(__FILE__ , __LINE__).stream()
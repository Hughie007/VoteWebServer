#pragma once
#include<string.h>
#include<assert.h>
#include "../varia.h"
const int BUFFERSIZE=4096;
const int maxLeaveRoom=32;
const char digits[]="9876543210123456789";
template<int SIZE>
class FixedBuffer:public noncopyable{
public:
    FixedBuffer():cur_(data_){}
    ~FixedBuffer(){};
    void append(const char* information,size_t len){
        if(avail()>len){
            memcpy(cur_,information,len);
            cur_+=len;
        }
        else{
            abort();
        }
    }
    const char* data() const{return data_;}
    char* current(){return cur_;}
    int avail(){
        return static_cast<int>(end()-cur_);
    }
    int length() const{return static_cast<int>(cur_-data_);}
    void add(size_t len){cur_+=len;}
    void reset(){cur_=data_;}
    void bzero(){memset(data_,0,sizeof(data_));}
private:
    const char* end() const{return data_+sizeof(data_);}
    char data_[SIZE];
    char* cur_;
};
class LogStream:public noncopyable{
public:
    typedef FixedBuffer<BUFFERSIZE> Buffer;
    LogStream& operator<<(short);
    LogStream& operator<<(unsigned short);
    LogStream& operator<<(int);
    LogStream& operator<<(unsigned int);
    LogStream& operator<<(long);
    LogStream& operator<<(unsigned long);
    LogStream& operator<<(long long);
    LogStream& operator<<(unsigned long long);
    LogStream& operator<<(const char* cstr);
    LogStream& operator<<(char c);
    LogStream& operator<<(double);
    LogStream& operator<<(long double);
    LogStream& operator<<(const std::string& str);
    LogStream& operator<<(bool);
    void append(const char* information,size_t len);
    const Buffer& getBuffer() const{
        return buffer_;
    }
    void resetBuffer();
private:
    template<typename T>
    void formatInteger(T);
    Buffer buffer_;
};
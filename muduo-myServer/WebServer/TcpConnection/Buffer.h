#pragma once
#include<vector>
#include<assert.h>
#include<sys/uio.h>
#include<string.h>
#include<string>
#include"../varia.h"
class Buffer:public copyable{
private:
    std::vector<char> buffer_;
    size_t readIndex_;
    size_t writeIndex_;
    static const char kCRLF[];
public:
    static const size_t kinitRoom=1024;
    static const size_t kprepIndex=8;
    explicit Buffer(size_t kRoom=kinitRoom);
    ~Buffer(){};
    char* begin();
    const char* begin() const;
    const char* getReadLoc() const;
    char* getWriteLoc();
    const char* getWriteLoc() const;
    size_t getReadIndex() const;
    size_t getReadAble() const;
    size_t getWriteAble() const;
    void hasWritten(size_t len);
    void retrieveAll();
    void retrieve(size_t len);
    void retrieveUntil(const char* end);
    std::string retrieveAsString(size_t len);
    std::string retrieveAllString();
    void append(const char* data,size_t len);
    void append(const void* data,size_t len);
    void appendIn64(int64_t v);
    int64_t readIn64();
    size_t readFd(int fd,int* savedErro);
    void ensureWriteAble(size_t len);
private:
    void enlargeSpace(size_t len);
};
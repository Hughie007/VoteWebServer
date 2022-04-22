#pragma once
#include<string>
#include<vector>
#include<functional>
#include<assert.h>
#include"../Callback.h"
#include"../syncTools/Mutex.h"
#include"../syncTools/Condition.h"
#include"../syncTools/CountDownLatch.h"
#include"../Thread.h"
template<int SIZE>
class FixedBuffer;
const int kLargeBuffer=4000*1000;
class AsyncLogging : public noncopyable{
public:
    AsyncLogging(const std::string basename,int flushIntercal=2);
    ~AsyncLogging();
    void append(const char* information,size_t len);
    void start();
    void stop();
    void end();
private:
    void threadFunc();
    typedef FixedBuffer<kLargeBuffer> Buffer;
    typedef std::shared_ptr<Buffer> BufferPtr;
    typedef std::vector<BufferPtr> BufferVector;
    const int flushInterval_;                   
    bool running_;
    std::string basename_;
    Thread thread_;
    MutexLock mutex_;
    Condition cond_;
    CountDownLatch latch_;
    BufferPtr currentBuffer_;
    BufferPtr nextBuffer_;
    BufferVector buffers_;
};
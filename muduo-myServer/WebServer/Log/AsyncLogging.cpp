#include "AsyncLogging.h"
#include"LogFile.h"
#include"LogStream.h"


AsyncLogging::AsyncLogging(const std::string basename,int flushInterval)
    :basename_(basename),
     running_(false),
     flushInterval_(flushInterval),
     mutex_(),
     cond_(mutex_),
     latch_(1),
     currentBuffer_(new Buffer),
     nextBuffer_(new Buffer),
     buffers_(),
     thread_(std::bind(&AsyncLogging::threadFunc,this),"Logging"){
         currentBuffer_->bzero();
         nextBuffer_->bzero();
         buffers_.reserve(16);
     }
AsyncLogging::~AsyncLogging(){
    if(running_){
        stop();
    }
}
void AsyncLogging::append(const char* information,size_t len){
    MutexLockGuard lock(mutex_);
    if(currentBuffer_->avail()>len){
        currentBuffer_->append(information,len);
    }
    else{
        buffers_.push_back(currentBuffer_);                      
        if(nextBuffer_){
            currentBuffer_=std::move(nextBuffer_);
        }
        else{
            currentBuffer_.reset(new Buffer);
        }
        currentBuffer_->append(information,len);
    }
    cond_.notifyall();                 //告诉Asyncogging的threadFunc有LOG出现
}
void AsyncLogging::threadFunc(){
    assert(running_==true);
    latch_.countDown();
    BufferPtr newBuffer1(new Buffer);
    BufferPtr newBuffer2(new Buffer);
    BufferVector buffersToWrite;
    newBuffer1->bzero();
    newBuffer2->bzero();
    buffersToWrite.reserve(16);
    LogFile Output(basename_);
    while(running_){
        assert(newBuffer1&&newBuffer1->length()==0);
        assert(newBuffer2&&newBuffer2->length()==0);
        assert(buffersToWrite.empty());
        {
            MutexLockGuard lock(mutex_);
            if(buffers_.empty()){
                cond_.waitForSeconds(flushInterval_);         //设置条件变量是因为如果有LOG，跳出此语句执行后续。
            }
            buffers_.push_back(currentBuffer_);
            currentBuffer_=std::move(newBuffer1);
            if(!nextBuffer_){                                 //在currentBuffer满了后，nextBuffer会被赋给currentBuffer，此时nextBuffer会变空
                nextBuffer_=std::move(newBuffer2);
            }
            buffersToWrite.swap(buffers_);
        }
        if(buffersToWrite.size()>25){
            char buf[256];
            time_t rawtime;
            struct tm *info;
            char buffer[80];
            time( &rawtime );
            info = localtime( &rawtime );
            snprintf(buf,sizeof buf,"throw out surplus messeges at %s,  %zd messeges are throwed out\n",
                asctime(info),buffersToWrite.size()-2
            );
            fputs(buf,stderr);
            Output.append(buf,sizeof buf);
            buffersToWrite.erase(buffersToWrite.begin()+2,buffersToWrite.end());
        }
        for(int i=0;i<buffersToWrite.size();i++){
            Output.append(buffersToWrite[i]->data(),buffersToWrite[i]->length());
        }
        if(buffersToWrite.size()>2){
            buffersToWrite.resize(2);
        }
        if(!newBuffer1){                                        //如果newBuffer1赋给了currentBuffer，则将buffersToWrite的元素赋给newBuffer1
            assert(!buffersToWrite.empty());
            newBuffer1=buffersToWrite.back();
            buffersToWrite.pop_back();
            newBuffer1->reset();
        }
        //Hb:如果newBuffer2为空，则bufferToWrite应该至少有2个元素
        if(!newBuffer2){
            assert(!buffersToWrite.empty());
            newBuffer2=buffersToWrite.back();
            buffersToWrite.pop_back();
            newBuffer2->reset();
        }
        Output.flush();                            //刷新输出缓冲区
        buffersToWrite.clear();
    }
    Output.flush();
}
void AsyncLogging::start(){
    running_=true;
    thread_.start();
    latch_.wait();                    //确保threadFunc跑起来后退出
}
void AsyncLogging::stop(){
    running_=false;
    cond_.notify();
    thread_.join();
}
#include<sys/time.h>
#include"logging.h"
#include"AsyncLogging.h"
std::string Logger::LogFileName_="MyServer.log";
static std::unique_ptr<AsyncLogging> asyncLogPtr;
static pthread_once_t once_control=PTHREAD_ONCE_INIT;
void once_init(){
    asyncLogPtr.reset(new AsyncLogging(Logger::getLogFileName()));
    asyncLogPtr->start();
}
void output(const char* information,size_t len){
    pthread_once(&once_control,once_init);              //创建Asyncogging
    asyncLogPtr->append(information,len);              //将Buffer内容写入Asyncogging的currentBuffer
}
Logger::Logger(const char* filename,int line)
    :LSA(filename,line)
    {}
Logger::LogStreamAppend::LogStreamAppend(const char* filename,int line)
    :basename_(filename),
     line_(line),
     stream_()
     {
         formatTime();
     }
Logger::LogStreamAppend::~LogStreamAppend(){}

void Logger::LogStreamAppend::formatTime(){
    struct timeval tv;
    __time_t time;
    char str_t[26]={0};
    gettimeofday(&tv,NULL);
    time=tv.tv_sec;
    struct tm* p_time=localtime(&time);
    strftime(str_t,26,"%Y-%m-%d %H:%M:%S\n",p_time);
    stream_<<str_t;
}
void Logger::setLogFileName(std::string str){
    LogFileName_=str;
}
std::string Logger::getLogFileName(){
    return LogFileName_;
}
LogStream& Logger::stream(){
    return LSA.stream_;
}
Logger::~Logger(){
    LSA.stream_<<" -- "<<LSA.basename_<<" -- "<<LSA.line_<<"\n";
    const LogStream::Buffer& buffer(LSA.stream_.getBuffer());
    output(buffer.data(),buffer.length());
}

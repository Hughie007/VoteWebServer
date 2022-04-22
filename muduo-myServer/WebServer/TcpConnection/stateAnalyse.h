#pragma once
#include<functional>
#include<stdarg.h>
#include"../varia.h"
#include"State.h"
class Buffer;
class Context;
class EventLoop;
class stateAnalyse:public noncopyable{
public:
    typedef std::function<void()> func;
    stateAnalyse(Buffer& Ibuffer,Buffer& Obuffer,EventLoop* loop,int sta_belongToLoop);
    void loadDataFromBuf();
    void analyseDataFromConn();
    void setFunc(const func& cb){startWriting=cb;}
    bool process_write(AnalyseData::ANALYSE_RESULT ret);
private:
    bool add_response(const char*format,...);
    bool add_status_line(int status,const char* title);
    bool add_headers(int content_len);
    bool add_content_length(int content_len);
    bool add_linger();
    bool add_blank_line();
    bool add_content(const char* content);
    func startWriting;                      //绑定TcpConn的写建立函数
    int sta_belongToLoop_;
    struct AnalyseData analyseData_;
    std::shared_ptr<Context> pcontext_ ;
    Buffer& Ibuffer_;
    Buffer& Obuffer_;
    EventLoop* loop_;
};

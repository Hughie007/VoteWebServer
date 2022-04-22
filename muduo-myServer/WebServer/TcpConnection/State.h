#pragma once
#include<memory>
#include<iostream>
#include<string.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<sys/mman.h>
#include"../varia.h"
struct AnalyseData{
    enum METHOD{
        GET=0,
        POST
    };
    enum ANALYSE_RESULT{
        BAD_REQUESE=0,
        NO_RESOURSE,
        FORBIDDEN_REQUESE,
        NO_REQUESE,
        FILE_REQUESE
    };
    AnalyseData();
    char* url_;
    char* file_address_;
    char* version_;
    char* host_;
    char* contextData_;
    bool linger_;
    int contextLength_;
    long fileSize;
    struct stat file_state;
    int voteOption[2];
    METHOD method_;
    ANALYSE_RESULT requeseLabel_;
};
class Context;

class State:public noncopyable{
public:
    virtual void handle(Context* pContext)=0;
    ~State()=default;
protected:
    State(){}
};

class Context:public noncopyable{
private:
    struct AnalyseData& analyseData_;
    int contextLength_;
    std::shared_ptr<State> state_;
    char data_[1024];
    int startIdx_;
    int m_checkIdx_;
    int m_dataEnd_;
    int belongToLoop_;
    bool endLabel_;
public:
    Context(const char* data,size_t len,AnalyseData& analyseData,int belongToLoop);
    char* getSentence();
    char* getLastSentence(){return data_+m_checkIdx_;}
    void Request();
    void ChangeState(State* state);
    int getContextOrder(){return belongToLoop_;}
    void setMethod(char* method){
        if(strcasecmp(method,"GET")==0){
            analyseData_.method_=AnalyseData::METHOD(0);
        }
        else if(strcasecmp(method,"POST")==0){
            analyseData_.method_=AnalyseData::METHOD(1);
        }
    }
    void setVersion(char* version){
        analyseData_.version_=version;
    }
    void setRequeseLabel(int val){
        analyseData_.requeseLabel_=AnalyseData::ANALYSE_RESULT(val);
    }
    void setEndLabel(bool val){
        endLabel_=val;
    }
    void setUrl(char* url){
        analyseData_.url_=url;
    }
    void setLinger(bool val){
        analyseData_.linger_=val;
    }
    void setHost(char* host){
        analyseData_.host_=host;
    }
    void setContextLength(int length){
        analyseData_.contextLength_=length;
    }
    void setContextData(char* contextData){
        analyseData_.contextData_=contextData;
    }
    void setFileAddress(long size,int fd){
        analyseData_.file_address_=(char*)mmap(0,size,PROT_READ,MAP_PRIVATE,fd,0);    //映射文件fd至内存file_address_
    }
    void setFileSize(long size){
        analyseData_.fileSize=size;
    }
    void setVoteOption(int voteWhat,int option){
        analyseData_.voteOption[0]=voteWhat;
        analyseData_.voteOption[1]=option;
    }
    void setFileState(struct stat state){
        analyseData_.file_state=state;
    }
    char* getContextData(){
        return analyseData_.contextData_;
    }
    bool getEndLabel(){return endLabel_;}
    const char* getUrl(){
        return analyseData_.url_;
    }
};

class StateEnd:public State{
public:
    StateEnd(){};
    ~StateEnd()=default;
    virtual void handle(Context* pContext){}
};

class StateRequese:public State{
public:
    StateRequese(){};
    ~StateRequese()=default;
    virtual void handle(Context* pContext);
};

class StateHeaders:public State{
public:
    StateHeaders(){};
    ~StateHeaders()=default;
    virtual void handle(Context* pContext);
};

class StateContexts:public State{
public:
    StateContexts(){};
    ~StateContexts()=default;
    virtual void handle(Context* pContext);
};

class StateDoRequese:public State{
public:
    StateDoRequese(){};
    ~StateDoRequese()=default;
    virtual void handle(Context* pContext);
};
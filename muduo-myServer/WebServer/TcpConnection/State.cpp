#include"State.h"
#include"../SocketOps.h"
#include"../Log/logging.h"
AnalyseData::AnalyseData()
    :url_(nullptr),
     version_(nullptr),
     host_(nullptr),
     file_address_(nullptr),
     linger_(false),
     contextLength_(0),
     fileSize(0),
     requeseLabel_(BAD_REQUESE){
         voteOption[0]=-1;
         voteOption[1]=-1;
     }
Context::Context(const char* data,size_t len,AnalyseData& analyseData,int belongToLoop)
    :startIdx_(0),
     m_checkIdx_(0),
     endLabel_(false),
     contextLength_(0),
     m_dataEnd_(len+1),
     analyseData_(analyseData),
     belongToLoop_(belongToLoop),
     state_(new StateRequese()){
         snprintf(data_,len+1,"%s\n",data);
     }

char* Context::getSentence(){
    int startIdx=startIdx_;
    for(;m_checkIdx_<m_dataEnd_;m_checkIdx_++){
        char temp=data_[m_checkIdx_];
        if(temp=='\r'){
            if(data_[m_checkIdx_+1]=='\n'){
                data_[m_checkIdx_++]='\0';
                data_[m_checkIdx_++]='\0';
                startIdx_=m_checkIdx_;
                return data_+startIdx;
            }
        }
    }
    return nullptr;
}
void Context::Request(){
    state_->handle(this);
}

void Context::ChangeState(State* state){
    state_.reset(state);
}
void StateRequese::handle(Context* pcontext){
    char* text=pcontext->getSentence();
    if(text==nullptr){
        pcontext->setRequeseLabel(0);
        pcontext->setEndLabel(true);
        pcontext->ChangeState(new StateEnd());
        return;
    }
    int len=strlen(text);
    char* start=text;
    start+=strspn(start," \t");     //去掉多余的空格
    int lenn=strspn(text," \t");
    char* cur=strpbrk(text," \t");
    if(cur==nullptr){
        pcontext->setRequeseLabel(0);
        pcontext->setEndLabel(true);
        pcontext->ChangeState(new StateEnd());
        return;
    }
    *cur++='\0';
    char* method=start;
    cur+=strspn(cur," \t");
    start=cur;
    if(strcasecmp(method,"GET")==0||strcasecmp(method,"POST")==0){
        pcontext->setMethod(method);
    }
    else{
        pcontext->setRequeseLabel(0);
        pcontext->setEndLabel(true);
        pcontext->ChangeState(new StateEnd());
        return;
    }
    cur=strpbrk(cur," \t");
    if(cur==nullptr){
        pcontext->setRequeseLabel(0);
        pcontext->setEndLabel(true);
        pcontext->ChangeState(new StateEnd());
        return;
    }
    *cur++='\0';
    char* url=start;
    cur+=strspn(cur," \t");
    start=cur;
    char* version=start;
    if(strcasecmp(version,"HTTP/1.1")==0){
        pcontext->setVersion(version);
    }
    else{
        pcontext->setRequeseLabel(0);
        pcontext->setEndLabel(true);
        pcontext->ChangeState(new StateEnd());
        return;
    }
    if(strncasecmp(url,"http://",7)==0){
        url+=7;
        url=strchr(url,'/');
    }
    else if(strncasecmp(url,"https://",8)==0){
        url+=8;
        url=strchr(url,'/');
    }
    if(!url||url[0]!='/'){
        pcontext->setRequeseLabel(0);
        pcontext->setEndLabel(true);
        pcontext->ChangeState(new StateEnd());
        return;
    }
    pcontext->setUrl(url);

    pcontext->ChangeState(new StateHeaders());
    return;
} 
void StateHeaders::handle(Context* pcontext){
    char* text=pcontext->getSentence();
    if(text==nullptr){
        pcontext->ChangeState(new StateDoRequese());
        return;
    }
    if(strncasecmp(text,"Host:",5)==0){
        text+=5;
        text+=strspn(text," \t");
        pcontext->setHost(text);
    }
    else if(strncasecmp(text,"Connection:",11)==0){
        text+=11;
        text+=strspn(text," \t");
        if(strcasecmp(text,"keep-alive")==0){
            pcontext->setLinger(true);
        }
    }
    else if(strncasecmp(text,"Content-length:",15)==0){
        text+=15;
        text+=strspn(text," \t");
        int contextLength=atol(text);
        pcontext->setContextLength(contextLength);
        if(contextLength==0){
            pcontext->ChangeState(new StateDoRequese());
            return;
        }
        else{
            pcontext->ChangeState(new StateContexts());
            return;
        }
    }
    else{
        LOG<<("unknow header:%s",text);
    }
    pcontext->ChangeState(new StateHeaders());
    return;
} 
void StateContexts::handle(Context* pcontext){
    char* text=pcontext->getLastSentence();
    pcontext->setContextData(text);
    pcontext->ChangeState(new StateDoRequese());
    return;
}
void StateDoRequese::handle(Context* pcontext){                //根据url_让服务器作出相应响应
    char* server_path=(char*)malloc(sizeof(char)*200);
    getcwd(server_path,200);
    char file_path[200]={0};
    strcpy(file_path,server_path);
    free(server_path);
    strcat(file_path,"/root");
    int len=strlen(file_path);
    const char* url=pcontext->getUrl();
    const char* urlRecord=strrchr(url,'/');
    if(*(urlRecord+1)=='0'){                                     //进入运动投票
        char* urlBack=(char*)malloc(sizeof(char)*200);
        strcpy(urlBack,"/sportVote.html");
        strncpy(file_path+len,urlBack,strlen(urlBack));
        free(urlBack);
    }
    else if(*(urlRecord+1)=='1'){                                //进入水果投票
        char* urlBack=(char*)malloc(sizeof(char)*200);
        strcpy(urlBack,"/fruitVote.html");
        strncpy(file_path+len,urlBack,strlen(urlBack));
        free(urlBack);
    }
    else if(*(urlRecord+1)=='2'){                                 //运动投票
        char* contextData=pcontext->getContextData();
        pcontext->setVoteOption(0,contextData[0+9]-'0');
        char* urlBack=(char*)malloc(sizeof(char)*200);
        strcpy(urlBack,"/thank.html");
        strncpy(file_path+len,urlBack,strlen(urlBack));
        free(urlBack);
    }
    else if(*(urlRecord+1)=='3'){                                 //水果投票
        char* contextData=pcontext->getContextData();
        pcontext->setVoteOption(1,contextData[0+9]-'0');
        char* urlBack=(char*)malloc(sizeof(char)*200);
        strcpy(urlBack,"/thank.html");
        strncpy(file_path+len,urlBack,strlen(urlBack));
        free(urlBack);
    }
    else if(*(urlRecord+1)=='4'){                                 //返回初始界面
        char* urlBack=(char*)malloc(sizeof(char)*200);
        strcpy(urlBack,"/welcome.html");
        strncpy(file_path+len,urlBack,strlen(urlBack));
        free(urlBack);
    }
    else if(*(urlRecord+1)=='5'){                                 //查看运动统计
        char* urlBack=(char*)malloc(sizeof(char)*200);
        std::string sportPath="/sportVoteSituation"+std::to_string(pcontext->getContextOrder());
        sportPath+=".html";
        strcpy(urlBack,sportPath.c_str());
        strncpy(file_path+len,urlBack,strlen(urlBack));
        free(urlBack);
    }
    else if(*(urlRecord+1)=='6'){                                 //查看水果统计
        char* urlBack=(char*)malloc(sizeof(char)*200);
        std::string fruitPath="/fruitVoteSituation"+std::to_string(pcontext->getContextOrder());
        fruitPath+=".html";
        strcpy(urlBack,fruitPath.c_str());
        strncpy(file_path+len,urlBack,strlen(urlBack));
        free(urlBack);
    }
    else if(*(urlRecord+1)=='7'){                                 //返回初始界面
        char* urlBack=(char*)malloc(sizeof(char)*200);
        strcpy(urlBack,"/welcome.html");
        strncpy(file_path+len,urlBack,strlen(urlBack));
        free(urlBack);
    }
    else if(*(urlRecord+1)=='\0'){
        char* urlBack=(char*)malloc(sizeof(char)*200);
        strcpy(urlBack,"/welcome.html");
        strncpy(file_path+len,urlBack,strlen(urlBack));
        free(urlBack);
    }
    else{
        strncpy(file_path+len,urlRecord,strlen(urlRecord));
    }
    struct stat file_state;
    if(stat(file_path,&file_state)<0){
        pcontext->setRequeseLabel(1);
        pcontext->setEndLabel(true);
        pcontext->ChangeState(new StateEnd());
        return;
    }
    if(!(file_state.st_mode&S_IROTH)){            //文件不可读
        pcontext->setRequeseLabel(2);
        pcontext->setEndLabel(true);
        pcontext->ChangeState(new StateEnd());
        return;
    }
    if(S_ISDIR(file_state.st_mode)){
        pcontext->setRequeseLabel(0);
        pcontext->setEndLabel(true);
        pcontext->ChangeState(new StateEnd());
        return;
    }
    pcontext->setFileState(file_state);
    int fd=open(file_path,O_RDONLY);
    pcontext->setFileSize(file_state.st_size);
    pcontext->setFileAddress(file_state.st_size,fd);
    close(fd);
    pcontext->setRequeseLabel(4);
    pcontext->setEndLabel(true);
    pcontext->ChangeState(new StateEnd());
    return;
}

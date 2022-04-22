#include"stateAnalyse.h"
#include"Buffer.h"
#include"../Log/logging.h"
#include"../Reactor/EventLoop.h"
const char *ok_200_title = "OK";
const char *error_400_title = "Bad Request";
const char *error_400_form = "Your request has bad syntax or is inherently impossible to staisfy.\n";
const char *error_403_title = "Forbidden";
const char *error_403_form = "You do not have permission to get file form this server.\n";
const char* error_404_title="Not found!";
const char *error_404_form = "The requested file was not found on this server.\n";
stateAnalyse::stateAnalyse(Buffer& Ibuffer,Buffer& Obuffer,EventLoop* loop,int sta_belongToLoop)
    :Ibuffer_(Ibuffer),
     Obuffer_(Obuffer),
     sta_belongToLoop_(sta_belongToLoop),
     loop_(loop){
     }
void stateAnalyse::loadDataFromBuf(){
    int readAble=Ibuffer_.getReadAble();
    pcontext_.reset(new Context(Ibuffer_.retrieveAllString().c_str(),readAble,analyseData_,sta_belongToLoop_));
}
bool stateAnalyse::process_write(AnalyseData::ANALYSE_RESULT ret){
    switch (ret)
    {
    case AnalyseData::ANALYSE_RESULT::BAD_REQUESE:
        add_status_line(400,error_400_title);
        add_headers(strlen(error_400_form));
        add_content(error_400_form);
        break;
    case AnalyseData::ANALYSE_RESULT::NO_RESOURSE:
        add_status_line(404,error_404_title);
        add_headers(strlen(error_404_form));
        add_content(error_404_form);
        break;
    case AnalyseData::ANALYSE_RESULT::FORBIDDEN_REQUESE:
        add_status_line(403,error_403_title);
        add_headers(strlen(error_403_form));
        add_content(error_403_form);
        break;
    case AnalyseData::ANALYSE_RESULT::NO_REQUESE:
        break;
    case AnalyseData::ANALYSE_RESULT::FILE_REQUESE:
        if(analyseData_.voteOption[0]!=-1){
            loop_->AddVote(analyseData_.voteOption[0],analyseData_.voteOption[1]);
            analyseData_.voteOption[0]=-1;                   //复原投票记录
            analyseData_.voteOption[1]=-1;
        }
        add_status_line(200,ok_200_title);
        add_headers(analyseData_.file_state.st_size);
        Obuffer_.append(analyseData_.file_address_,analyseData_.file_state.st_size);
        munmap(analyseData_.file_address_,analyseData_.fileSize);         //取消file_address_的映射
        analyseData_.file_address_=nullptr;
        
        return true;
    default:
        break;
    }
    return false;
}
void stateAnalyse::analyseDataFromConn(){
    loadDataFromBuf();
    while(!pcontext_->getEndLabel()){
        pcontext_->Request();
    }
    AnalyseData::ANALYSE_RESULT read_ret=analyseData_.requeseLabel_;
    if(read_ret==AnalyseData::ANALYSE_RESULT::NO_REQUESE){

    }
    bool write_ret=process_write(read_ret);
    if(write_ret==false){
        //感觉stateAnalyse设计不是很好，因为其和TcpConn的交互性比较差
    }
    startWriting();
}
bool stateAnalyse::add_response(const char* format,...){
    va_list arg_list;
    va_start(arg_list,format);
    char buf[200];
    int len=vsnprintf(buf,200,format,arg_list);
    Obuffer_.append(buf,len);
    va_end(arg_list);
    LOG<<("request:%s",buf);
    return true;
}
bool stateAnalyse::add_status_line(int status,const char* title){
    return add_response("%s %d %s\r\n",analyseData_.version_,status,title);
}
bool stateAnalyse::add_headers(int content_len){
    return add_content_length(content_len)&&add_linger()&&add_blank_line();
}
bool stateAnalyse::add_content_length(int content_len){
    return add_response("Content-Length:%d\r\n",content_len);
}
bool stateAnalyse::add_linger(){
    return add_response("Connection:%s\r\n",(analyseData_.linger_==true)?"keep-alive":"close");
}
bool stateAnalyse::add_blank_line(){
    return add_response("%s","\r\n");
}
bool stateAnalyse::add_content(const char* content){
    return add_response("%s",content);
}
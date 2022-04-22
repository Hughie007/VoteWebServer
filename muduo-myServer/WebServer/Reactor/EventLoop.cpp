#include "EventLoop.h"
#include"../TcpConnection/Channel.h"
#include"Epoll.h"
#include"../TimeManager/TimeStamp.h"
#include"../TcpConnection/TcpConn.h"
#include"../CurrentThread.h"
#include"../Pool/MysqlPool.h"
#include"../Log/logging.h"
<<<<<<< HEAD
=======
#include"../Pool/redisConnectionPool.h"
>>>>>>> 6d98924 (first common)
__thread EventLoop* pLoopInThisThread=0;
int createEventFd(){
    int evtfd=eventfd(0,EFD_NONBLOCK|EFD_CLOEXEC);
    if(evtfd<0){
        //Log<<....
        abort();   //中止程序
    }
    return evtfd;
}
EventLoop::EventLoop(int order)
    :looping_(false),
     quit_(false),
     callingPendingFunctors(false),
     wakeupFd_(createEventFd()),
     wakeupChan_(new Channel(this,wakeupFd_)),
     mutex_(),
     poller_(new Epoll()),
     timeManager_(11),          //注意，此处实际设置的心跳数为所设置数-1 
<<<<<<< HEAD
     voteUpdateInterval(0),
=======
     voteUpdateInterval(1),
>>>>>>> 6d98924 (first common)
     loopOrder_(order),
     threadId_(CurrentThread::tid()){
         if(pLoopInThisThread){
             //Log..
             abort();
         }
         else{
             pLoopInThisThread=this;
         }
         voteInit();
         wakeupChan_->setEvents(EPOLLIN||EPOLLET);
         wakeupChan_->setReadCallback(std::bind(&EventLoop::handleRead,this,std::placeholders::_1));
         wakeupChan_->enableReading();
     }
EventLoop::~EventLoop(){
    pLoopInThisThread=NULL;
    close(wakeupFd_);
}
bool EventLoop::isInLoopThread() const{
    return threadId_==CurrentThread::tid();
}
void EventLoop::abortNotInLoopThread(){
    //Log...
}
void EventLoop::assertInLoopThread(){
    if(!isInLoopThread()){
        abortNotInLoopThread();
    }
}
void EventLoop::loop(){
    assert(!looping_);
    assertInLoopThread();
    looping_=true;
    std::vector<sp_Chan> req_chan;
    while(!quit_){
        req_chan=poller_->poll();
        TimeStamp tsNow=TimeStamp::now();
        eventHandleing=true;
        for(auto i:req_chan){
            i->handleEvent(tsNow);
        }
        eventHandleing=false;
        doPendingFunctors();
        if(threadId_!=MainTid){
<<<<<<< HEAD
            if(voteUpdateInterval==10){     //如果间隔到了就更新投票情况
                UpdateVoteSituation();
=======
            if(voteUpdateInterval%1==0){      //如果间隔到了就更新数据库投票情况
                UpdateVoteSituation();
            }
            if(voteUpdateInterval%1==0){     //如果间隔到了就更新服务端投票情况
                InitVoteSituation();
            }
            if(voteUpdateInterval%1==0){
                updateHtml();
>>>>>>> 6d98924 (first common)
                voteUpdateInterval=0;
            }
            voteUpdateInterval++;              
            handleUnactiveConn();       //处理非活跃连接
        }
    }
    looping_=false;
}
void EventLoop::doPendingFunctors(){
    std::vector<Functor> functors;
    callingPendingFunctors=true;
    {
        MutexLockGuard lock(mutex_);              //此处需要加锁，因为其他线程可能通过runInLoop改变pendingFunctors
        functors.swap(pendingFunctors_);
    }
    for(int i=0;i<functors.size();i++){
        functors[i]();
    }
    callingPendingFunctors=false;
}
void EventLoop::runInLoop(const Functor& cb){
    if(isInLoopThread()){
        cb();
    }
    else{
        queueInLoop(cb);
    }
}
void EventLoop::queueInLoop(const Functor& cb){
    {
        MutexLockGuard lock(mutex_);
        pendingFunctors_.push_back(cb);
    }
    if(!isInLoopThread()||callingPendingFunctors){      //1.其他线程调用需唤醒wakeupfd  2.doPendingFunctors时也可能调用queueInLoop，此时也需唤醒
        wakeup();
    }
}
void EventLoop::wakeup(){
    uint64_t signalWord=1;
    size_t n = ::write(wakeupFd_,&signalWord,sizeof signalWord);        //注意套接字资源在线程间共享
    if(n!=sizeof(signalWord)){
        //Log...
    }
}
void EventLoop::quit(){
    quit_=true;
    if(!isInLoopThread()){
        wakeup();
    }
}
void EventLoop::addToPoller(std::shared_ptr<Channel> spChan){
    assert(spChan->ownerLoop()==this);
    poller_->epoll_add(spChan);
}
void EventLoop::updatePoller(std::shared_ptr<Channel> spChan){
    poller_->epoll_mod(spChan);
}
void EventLoop::removeFromPoller(std::shared_ptr<Channel> spChan){
    poller_->epoll_del(spChan);
}

class IgnoreSigPipe{                         
public:
    IgnoreSigPipe(){
        ::signal(SIGPIPE,SIG_IGN);
    }
};
void EventLoop::handleRead(TimeStamp ts){
    uint64_t one =1;
    ssize_t n=::read(wakeupFd_,&one,sizeof one);
    if(n!=sizeof one){
        //LOG...
    }
    wakeupChan_->setEvents(EPOLLIN|EPOLLET);
}
void EventLoop::addTimer(EntryPtr sp_Entry){
    timeManager_.addTimer(sp_Entry);
}
void EventLoop::setMysqlPool(Mysql_pool* mysqlPool){
    mysqlPool_=mysqlPool;
}
<<<<<<< HEAD
void EventLoop::InitVoteSituation(){
=======
void EventLoop::setRedisPool(CachePool* redisPool){
    redisPool_=redisPool;
}
void EventLoop::InitVoteSituation(){
    redisContext* redisPtr=nullptr;
    redisRAII rediscon(&redisPtr,redisPool_);
    redisReply* reply=(redisReply*)redisCommand(redisPtr,"MGET %s %s %s %s %s %s %s %s","vote00","vote01","vote02","vote03",\
                                                                                        "vote10","vote11","vote12","vote13");
    if(reply->type==REDIS_REPLY_ERROR){
        LOG<<"ERROR:"+(std::string)reply->str+"\n";
    }
    else if(reply->type!=REDIS_REPLY_ARRAY){
        LOG<<"Unexpected type:"+std::to_string(reply->type)+"\n";
    }
    else{
        for(int i=0;i<reply->elements;i++){
            voteSituation[i/4][i%4]=atoi((const char*)reply->element[i]->str);
        }
    }
    /*
>>>>>>> 6d98924 (first common)
    MYSQL* mysql=NULL;
    MysqlRAII mysqlcon(&mysql,mysqlPool_);
    if(mysql_query(mysql,"SELECT option1,option2,option3,option4 FROM user")){
        LOG<<"erron on connection to mysql";
    }
    MYSQL_RES *result=mysql_store_result(mysql);
    MYSQL_FIELD *fields=mysql_fetch_fields(result);
    for(int i=0;i<2;i++){
        MYSQL_ROW row=mysql_fetch_row(result);
        for(int j=0;j<4;j++){
            voteSituation[i][j]=atoi(row[j]);
        }
    }
<<<<<<< HEAD
}
void EventLoop::UpdateVoteSituation(){
    InitVoteSituation();
    MYSQL* mysql=NULL;
    MysqlRAII mysqlcon(&mysql,mysqlPool_);
    char strQuery[100]={0};             //更新数据库
    for(int i=0;i<2;i++){
        for(int j=0;j<4;j++){
            voteSituation[i][j]+=voteAdd[i][j];
            voteAdd[i][j]=0;
        }
    }
    snprintf(strQuery,sizeof(strQuery),"update user set option1=(%d),option2=(%d),option3=(%d),option4=(%d) where voteWhat=\"sport\"",voteSituation[0][0],voteSituation[0][1],voteSituation[0][2],voteSituation[0][3]);
    if(mysql_query(mysql,strQuery)){
        LOG<<"erron on connection to mysql";
    }
    char strQuery2[100]={0};
    snprintf(strQuery2,sizeof(strQuery2),"update user set option1=(%d),option2=(%d),option3=(%d),option4=(%d) where voteWhat=\"fruit\"",voteSituation[1][0],voteSituation[1][1],voteSituation[1][2],voteSituation[1][3]);
    if(mysql_query(mysql,strQuery2)){
        LOG<<"erron on connection to mysql";
    }
    updateHtml();
=======
    */
    for(int i=0;i<2;i++){
        for(int j=0;j<4;j++){
            voteAdd[i][j]=0;
        }
    }
}
void EventLoop::UpdateVoteSituation(){
    MYSQL* mysql=NULL;
    MysqlRAII mysqlcon(&mysql,mysqlPool_);
    char strQuery[150]={0};             //更新数据库
    snprintf(strQuery,sizeof(strQuery),"update user set option1=option1+(%d),option2=option2+(%d),option3=option3+(%d),option4=option4+(%d) where voteWhat=\"sport\"",voteAdd[0][0],voteAdd[0][1],voteAdd[0][2],voteAdd[0][3]);
    if(mysql_query(mysql,strQuery)){
        LOG<<"erron on connection to mysql";
    }
    char strQuery2[150]={0};
    snprintf(strQuery2,sizeof(strQuery2),"update user set option1=option1+(%d),option2=option2+(%d),option3=option3+(%d),option4=option4+(%d) where voteWhat=\"fruit\"",voteAdd[1][0],voteAdd[1][1],voteAdd[1][2],voteAdd[1][3]);
    if(mysql_query(mysql,strQuery2)){
        LOG<<"erron on connection to mysql";
    }
>>>>>>> 6d98924 (first common)
}
void EventLoop::updateHtml(){
    char updateBuffer[2048]={0};
    int len=0;

    //获取原sport网页的格式
    std::ifstream in("./root/sportVoteSituation.html");
    in.seekg(0,std::ios::end);
    int length=in.tellg();
    in.seekg(0,std::ios::beg);
    char* buffer=new char[length+10]();
    char* start=buffer;
    in.read(buffer,length);
    in.close();

    //根据数据制作新网页
    char* numStart;
    for(int i=0;i<4;i++){
        //找到数字开始的地方
        numStart=strpbrk(start,"{");
        snprintf(updateBuffer+len,numStart-start+2,"%s",start);            //snprintf的拷贝长度参数为实际长度+1，+1是给'\0'的
        len+=numStart-start+1;
        start=strpbrk(numStart,"}");
        //记录数字长度
        char numStr[10]={0};
        snprintf(numStr,sizeof(numStr),"%d",voteSituation[0][i]);
        int numLen=strlen(numStr);

        snprintf(updateBuffer+len,numLen+1,"%s",numStr);
        len+=numLen;
    }
    snprintf(updateBuffer+len,sizeof(updateBuffer),"%s",start);
    std::ofstream oFile;
    std::string sportPath="./root/sportVoteSituation"+std::to_string(loopOrder_);
    sportPath+=".html";
    oFile.open(sportPath,std::ios::out);
    oFile<<updateBuffer;
    delete[] buffer;
    oFile.close();

    memZero(updateBuffer,sizeof(updateBuffer));
    len=0;
    //获取原fruit网页的格式
    std::ifstream in2("./root/fruitVoteSituation.html");
    in2.seekg(0,std::ios::end);
    int length2=in2.tellg();
    in2.seekg(0,std::ios::beg);
    char* buffer2=new char[length2+10]();
    char* start2=buffer2;
    in2.read(buffer2,length2);
    in2.close();

    //根据数据制作新网页
    char* numStart2;
    for(int i=0;i<4;i++){
        //找到数字开始的地方
        numStart2=strpbrk(start2,"{");
        snprintf(updateBuffer+len,numStart2-start2+2,"%s",start2);
        len+=numStart2-start2+1;
        start2=strpbrk(numStart2,"}");
        //记录数字长度
        char numStr2[10]={0};
        snprintf(numStr2,sizeof(numStr2),"%d",voteSituation[1][i]);
        int numLen2=strlen(numStr2);

        snprintf(updateBuffer+len,numLen2+1,"%s",numStr2);
        len+=numLen2;
    }
    snprintf(updateBuffer+len,sizeof(updateBuffer),"%s",start2);
    std::ofstream oFile2;
    std::string fruitPath="./root/fruitVoteSituation"+std::to_string(loopOrder_);
    fruitPath+=".html";
    oFile2.open(fruitPath,std::ios::out);
    oFile2<<updateBuffer;
    delete[] buffer2;
    oFile2.close();
}
void EventLoop::AddVote(int voteWhat,int option){
    voteAdd[voteWhat][option]++;
}
void EventLoop::voteInit(){
    for(int i=0;i<2;i++){
        for(int j=0;j<4;j++){
            voteAdd[i][j]=0;
            voteSituation[i][j]=0;
        }
    }
}
IgnoreSigPipe initObj;              //忽略SIGPIPE,防止终止进程
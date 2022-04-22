#include"TcpServer.h"
#include"./Log/logging.h"
#include"./Reactor/Acceptor.h"
#include"./Pool/EventLoopThreadPool.h"
#include"./Reactor/EventLoop.h"
#include"InetAddress.h"
#include"./TcpConnection/TcpConn.h"
#include"./TimeManager/TimeStamp.h"
#include"./Pool/MysqlPool.h"
<<<<<<< HEAD
TcpServer::TcpServer(EventLoop *loop,const InetAddress& listenAddr)
    :loop_(loop),
     threadPool_(new EventLoopThreadPool(loop)),
     acceptor_(new Acceptor(loop,listenAddr,true)){
         acceptor_->setNewConnectCallback(std::bind(&TcpServer::newConnection,this,std::placeholders::_1,std::placeholders::_2));
     }
=======
#include"./Pool/redisConnectionPool.h"
#include"./DbSyn/dbSYN.h"
TcpServer::TcpServer(EventLoop *loop,const InetAddress& listenAddr)
    :loop_(loop),
    nextConnId_(0),
    acceptor_(new Acceptor(loop,listenAddr,true)),
    threadPool_(nullptr),
    dbSynTool(new dbSYN(3000)){
        acceptor_->setNewConnectCallback(std::bind(&TcpServer::newConnection,this,std::placeholders::_1,std::placeholders::_2));
        dbSynTool->setConn();
    }
>>>>>>> 6d98924 (first common)

TcpServer::~TcpServer(){}

void TcpServer::newConnection(int sockfd,const InetAddress& peerAddr){
    char buf[32];
    snprintf(buf,sizeof buf,"#%d",nextConnId_);
    ++nextConnId_;
    std::string connName=name_+buf;
    LOG<<"TcpServer get new connection:"<<connName<<" from "<<peerAddr.toIpPort();
    InetAddress localAddr(muduo::net::sockets::getLocalAddr(sockfd));
    EventLoop* ioLoop=threadPool_->getNextLoop();
    TcpConnPtr sp_TcpConn(new TcpConn(ioLoop,connName,sockfd,localAddr,peerAddr));
    connectionMap_[connName]=sp_TcpConn;
    sp_TcpConn->setConnectionCallback(connectionCallback_);
    sp_TcpConn->setMessageCallback(messageCallback_);
    sp_TcpConn->setCloseCallback(std::bind(&TcpServer::removeConnection,this,std::placeholders::_1));
    ioLoop->runInLoop(std::bind(&TcpConn::connectEstablished,sp_TcpConn));
}

void TcpServer::start(){
    mysqlPool_=Mysql_pool::getInstance();                     //获取连接池对象
<<<<<<< HEAD
    mysqlPool_->init("localhost","root","123456","yourdb2",3306,8);    //连接池连接至数据库
    threadPool_->start(mysqlPool_);                      //将连接池指针传递给每个loop，使得loop可以连接至数据库
=======
    mysqlPool_->init("localhost","root","123456","VoteSituation",3306,8);    //连接池连接至数据库
    redisPool_=CachePool::getInstance();
    redisPool_->init("127.0.0.1",6379,8);
    threadPool_=std::unique_ptr<EventLoopThreadPool>(new EventLoopThreadPool(loop_));
    dbSynTool->startSyn();
    threadPool_->start(mysqlPool_,redisPool_);                      //将连接池指针传递给每个loop，使得loop可以连接至数据库
>>>>>>> 6d98924 (first common)
    acceptor_->listen();
}

void TcpServer::removeConnection(const TcpConnPtr& sp_TcpConn){
    loop_->runInLoop(std::bind(&TcpServer::removeConnectionInLoop,this,sp_TcpConn));  
    //当此函数跳出时，TcpConn生命期结束
}

void TcpServer::removeConnectionInLoop(const TcpConnPtr& sp_TcpConn){
    loop_->assertInLoopThread();
    TimeStamp ts=TimeStamp::now();
    LOG<<("TcpServer::removeConnectionInLoop:[%s] at %s\n",sp_TcpConn->name().c_str(),ts.toFormatString().c_str());
    size_t n=connectionMap_.erase(sp_TcpConn->name());
<<<<<<< HEAD
    //assert(n==1);(void)n;         //没有使用n,告诉编译器不要发出提示警告       
=======
    //assert(n==1);(void)n;         //没有使用n,告诉编译器不要发出提示警告       总是assert会报错，不知道为啥
>>>>>>> 6d98924 (first common)
    if(n==0){
        return;
    }
    EventLoop* ioLoop=sp_TcpConn->getEventLoop();
    ioLoop->runInLoop(std::bind(&TcpConn::connectDestroyed,sp_TcpConn));
}

void TcpServer::setThreadNum(int num){
    threadPool_->setThreadNum(num);
}



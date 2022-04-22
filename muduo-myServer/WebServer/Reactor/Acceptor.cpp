#include"Acceptor.h"
#include"../TimeManager/TimeStamp.h"
#include"../InetAddress.h"
#include"../TcpConnection/Channel.h"
#include"EventLoop.h"
Acceptor::Acceptor(EventLoop* loop,const InetAddress& listenAddr,bool reusePort)
    :listening_(false),
     loop_(loop),
<<<<<<< HEAD
     acceptorFd_(muduo::net::sockets::createNonblockingOrDie(listenAddr.family())),  
=======
     acceptorFd_(muduo::net::sockets::createNonblockingOrDie(listenAddr.family())),  //不是很懂family保存的信息
>>>>>>> 6d98924 (first common)
     acceptorChannel_(new Channel(loop,acceptorFd_.fd())){
         acceptorFd_.setReuseAddr(true);
         acceptorFd_.setReusePort(reusePort);
         acceptorFd_.bindAddress(listenAddr);
         acceptorChannel_->setReadCallback(std::bind(&Acceptor::handleRead,this,std::placeholders::_1));
     }
Acceptor::~Acceptor(){}

void Acceptor::handleRead(TimeStamp ts){
    loop_->assertInLoopThread();
    InetAddress peerAddr(0);
    int connfd=acceptorFd_.accept(&peerAddr);
    if(newConnCallback_){
        newConnCallback_(connfd,peerAddr);
    }
    else{
        muduo::net::sockets::close(connfd);
    }
}
void appendLog(const char* information,size_t len);
void Acceptor::listen(){
    loop_->assertInLoopThread();
    listening_=true;
    acceptorFd_.listen();
    acceptorChannel_->enableReading();
}
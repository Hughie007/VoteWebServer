#include"TcpConn.h"
#include"../Reactor/EventLoop.h"
#include"Socket.h"
#include"Channel.h"
#include"../Log/logging.h"
#include"../TimeManager/TimeStamp.h"
#include"../Endian.h"
TcpConn::TcpConn(EventLoop* loop,std::string connName,int connfd,const InetAddress& localAddr,const InetAddress& peerAddr)
    :loop_(loop),
     name_(connName),
     connfd_(new Socket(connfd)),
     channel_(new Channel(loop,connfd_->fd())),
     localAddr_(localAddr),
     inputBuffer_(),
     outputBuffer_(),
     stateRecord_(inputBuffer_,outputBuffer_,loop_,loop->getOrder()),
     peerAddr_(peerAddr){
        channel_->setReadCallback(std::bind(&TcpConn::handleRead,this,std::placeholders::_1));
        channel_->setWriteCallback(std::bind(&TcpConn::handleWrite,this));
        channel_->setCloseCallback(std::bind(&TcpConn::handleClose,this));
        stateRecord_.setFunc(std::bind(&TcpConn::writeEstablished,this));
     }
TcpConn::~TcpConn(){
    //Log...
    LOG<< "TcpConnection::dtor[" <<  name_ << "] at " << this
            << " fd=" << channel_->fd();
    assert(state_ == kDisconnected);
}
void TcpConn::shutDown(){
    if(state_==kConnected){
        setState(kDisconnecting);
        loop_->runInLoop(std::bind(&TcpConn::shutDownInLoop,this));
    }
}
void TcpConn::shutDownInLoop(){
    loop_->assertInLoopThread();
    if(!channel_->isWriting()){             //等待outputBuffer_未发送数据发送完
        connfd_->shotDownWrite();
    }
}
void TcpConn::connectEstablished(){
    setState(kConnected);
    connectionCallback_(shared_from_this());
    channel_->enableReading();
}
void TcpConn::writeEstablished(){
    channel_->enableWriting();
}
void TcpConn::connectDestroyed(){
    loop_->assertInLoopThread();
    assert(state_==kConnected || state_== kDisconnecting);
    setState(kDisconnected);
    loop_->removeFromPoller(channel_);
    printf("EXIT\n");
}
void TcpConn::handleRead(TimeStamp ts){
    int savedError=0;
    ssize_t n=inputBuffer_.readFd(channel_->fd(),&savedError);
    if(n>0){
        messageCallback_(shared_from_this(),&inputBuffer_,ts);
        //channel_->enableReading();
    }
    else if(n==0){
        channel_->disableAll();
    }
    else{
        //handleClose();
    }
}
void TcpConn::handleWrite(){
    loop_->assertInLoopThread();
    if(channel_->isWriting()){
        ssize_t n=::write(channel_->fd(),outputBuffer_.getReadLoc(),outputBuffer_.getReadAble());
        if(n>0){
            outputBuffer_.retrieve(n);
            if(outputBuffer_.getReadAble()==0){
                channel_->disableWriting();
                if(state_==kDisconnecting){           //当outputBuffer_缓冲区数据发送完后执行未完成的shutDown操作
                    shutDownInLoop();                 
                }
            }
            else{
                LOG<<"I am going to write more data";
            }
        }
        else{
            LOG<<"TcpConn::handleWrite is wrong!";
        }
    }
    else{
        LOG<<"no more writing";
    }
}
void TcpConn::handleClose(){
    closeCallback_(shared_from_this());
}
void TcpConn::send(const std::string& message){
    if(state_==kConnected){
        if(loop_->isInLoopThread()){
            sendInLoop(message);
        }
        else{
            loop_->runInLoop(std::bind(&TcpConn::sendInLoop,this,message));
        }
    }
}
void TcpConn::sendInLoop(const std::string& message){
    loop_->assertInLoopThread();
    ssize_t nwrite;
    if(!channel_->isWriting() && outputBuffer_.getReadAble()==0){
        nwrite=::write(channel_->fd(),message.data(),message.size());
        if(nwrite>=0){
            if(implicit_cast<size_t>(nwrite)<message.size()){
                LOG<<"I am going to recieve more message.";
            }
        }
        else{
            nwrite=0;
            if(errno!=EWOULDBLOCK){
                LOG<<"TcpConn::sendInLoop is wrong!";
            }
        }
    }
    assert(nwrite>=0);
    if(implicit_cast<size_t>(nwrite)<message.size()){
        outputBuffer_.append(message.data(),message.size()-nwrite);
        if(!channel_->isWriting()){
            channel_->enableWriting();
        }
    }
}
bool TcpConn::connected() const{
    return state_==kConnected;
}
void TcpConn::setTcpNoDelay(bool on){
    connfd_->setTcpNoDelay(on);
}
void TcpConn::addTimer(){
    EntryPtr newEntryPtr(wp_Entry_.lock());      //此时，TcpConn已被设置
    if(newEntryPtr)
        loop_->addTimer(newEntryPtr);
}
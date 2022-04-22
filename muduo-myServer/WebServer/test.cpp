#include<iostream>
#include<memory>
#include"SocketOps.h"
#include"CurrentThread.h"
#include"InetAddress.h"
#include"./Reactor/EventLoop.h"
#include"./TcpConnection/TcpConn.h"
#include"./TimeManager/TimeWheel.h"
#include"./TimeManager/TimeStamp.h"
#include"TcpServer.h"
<<<<<<< HEAD

=======
/*
void threadFunc(){
    std::cout<<"pid="<<getpid()<<",tid="<<CurrentThread::tid()<<std::endl;
    EventLoop loop_;
    loop_.loop();
}
*/
>>>>>>> 6d98924 (first common)
void newConnection(int sockfd,const InetAddress& peeraddr){
    printf("newConnection():accepted a new connection from %s\n",peeraddr.toIpPort().c_str());
    ::write(sockfd,"Hello!\n",7);
    muduo::net::sockets::close(sockfd);
}
void onConnection(const std::shared_ptr<TcpConn>& sp_TcpConn){
    if(sp_TcpConn->connected()){
        printf("onConnection（）: new connection [%s] from %s\n",sp_TcpConn->name().c_str(),sp_TcpConn->getInetAddress().toIpPort().c_str());
<<<<<<< HEAD
=======
        //sp_TcpConn->send("Hello,how are you?");
        //sp_TcpConn->send("I love you.\n");
        //sp_TcpConn->shutDown();
>>>>>>> 6d98924 (first common)
        EntryPtr entryPtr(new Entry(sp_TcpConn));
        weakEntryPtr wp_Entry(entryPtr);
        sp_TcpConn->setWeakEntryPtr(wp_Entry);              //给Tcponn设置Entry的弱引用，方便后续读事件的重新计时
        sp_TcpConn->addTimer();                      //加入定时监控时间轮
    }
}
<<<<<<< HEAD
void onMessage(const std::shared_ptr<TcpConn>& sp_TcpConn,Buffer* buf,TimeStamp ts){
    printf("onMessage():received %zd bytes from connection [%s] at %s\n",buf->getReadAble(),sp_TcpConn->name().c_str(),ts.toFormatString().c_str());
=======
void onMessage2(const std::shared_ptr<TcpConn>& sp_TcpConn,const char* data,ssize_t len){
    printf("onMessage():received %zd bytes from connection:[%s]\n",len,sp_TcpConn->name().c_str());
}
void onMessage(const std::shared_ptr<TcpConn>& sp_TcpConn,Buffer* buf,TimeStamp ts){
    printf("onMessage():received %zd bytes from connection [%s] at %s\n",buf->getReadAble(),sp_TcpConn->name().c_str(),ts.toFormatString().c_str());
    //printf("onMessage():[%s]\n",buf->retrieveAllString().c_str());
>>>>>>> 6d98924 (first common)
    sp_TcpConn->addTimer();                          //客户发送信息，更新对应时间轮
    sp_TcpConn->getStateRecord_().analyseDataFromConn();
}
int MainTid=CurrentThread::tid();
int main(){
<<<<<<< HEAD
=======
    /*
    std::cout<<"main():pid="<<getpid()<<",tid="<<CurrentThread::tid()<<std::endl;
    EventLoop loop_;
    Thread thread(threadFunc);
    thread.start();
    loop_.loop();
    pthread_exit(NULL);
    */

    /*
    printf("main():pid=%d\n",getpid());
    EventLoop loop;
    InetAddress listenAddr(9981);
    Acceptor ac(&loop,listenAddr,true);
    ac.setNewConnectCallback(newConnection);
    ac.listen();
    loop.loop();
    */

        
>>>>>>> 6d98924 (first common)
    printf("main():pid=%d\n",getpid());
    InetAddress listenAddr(9981);
    EventLoop loop(-1);
    TcpServer server(&loop,listenAddr);
    server.setConnectionCallback(onConnection);
    server.setMessageCallback(onMessage); 
    server.start();
    loop.loop();
<<<<<<< HEAD
=======
    
>>>>>>> 6d98924 (first common)
    return 0;
}
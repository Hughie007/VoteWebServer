#pragma once
#include<map>
#include<functional>
#include"varia.h"
#include"Callback.h"
class EventLoop;
class EventLoopThreadPool;
class Acceptor;
class InetAddress;
class Mysql_pool;
<<<<<<< HEAD
=======
class CachePool;
class dbSYN;
>>>>>>> 6d98924 (first common)
class TcpServer:public noncopyable{
public:
    TcpServer(EventLoop *loop,const InetAddress& listenAddr);
    ~TcpServer();
    void start();
    void setConnectionCallback(const ConnectionCallback& cb){connectionCallback_=cb;}
    void setMessageCallback(const MessageCallback& cb){messageCallback_=cb;}
    void removeConnection(const TcpConnPtr& sp_TcpConn);
    void removeConnectionInLoop(const TcpConnPtr& sp_TcpConn);
    void setThreadNum(int num);
private:
    typedef std::map<std::string,TcpConnPtr> ConnectionMap;
    void newConnection(int sockfd,const InetAddress& peerAddr);
    EventLoop *loop_;
    const std::string name_;
    std::unique_ptr<Acceptor> acceptor_;
<<<<<<< HEAD
    std::unique_ptr<EventLoopThreadPool> threadPool_;;
=======
    std::unique_ptr<EventLoopThreadPool> threadPool_;
    std::unique_ptr<dbSYN> dbSynTool;
>>>>>>> 6d98924 (first common)
    ConnectionCallback connectionCallback_;
    MessageCallback messageCallback_;
    bool started_;
    int nextConnId_;
    ConnectionMap connectionMap_;          //存储每个连接的生命
<<<<<<< HEAD
    Mysql_pool* mysqlPool_;
=======
    Mysql_pool* mysqlPool_;          
    CachePool* redisPool_;                
>>>>>>> 6d98924 (first common)
};
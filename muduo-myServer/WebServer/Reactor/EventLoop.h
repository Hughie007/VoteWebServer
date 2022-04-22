#pragma once
#include<memory>
#include<functional>
#include <sys/eventfd.h>
#include <sys/epoll.h>
#include <sys/unistd.h>
#include <iostream>
#include <mysql/mysql.h>
#include<poll.h>
#include<signal.h>
#include<vector>
#include<ios>
#include<fstream>
#include<string.h>
#include"../syncTools/Mutex.h"
#include"../Callback.h"
#include"../TimeManager/TimeManager.h"
class TimeStamp;
class Channel;
class Epoll;
class TimeManager;
class Mysql_pool;
<<<<<<< HEAD
=======
class CachePool;
>>>>>>> 6d98924 (first common)
extern int MainTid;
class EventLoop:public noncopyable{
public:
    typedef std::function<void()> Functor;
    EventLoop(int order);
    ~EventLoop();
    void loop();
    bool isInLoopThread() const;
    void assertInLoopThread();
    void runInLoop(const Functor&cb);
    void queueInLoop(const Functor& cb);
    void wakeup();
    void quit();
    void addToPoller(std::shared_ptr<Channel> spChan);
    void updatePoller(std::shared_ptr<Channel> spChan);
    void removeFromPoller(std::shared_ptr<Channel> spChan);
    void addTimer(EntryPtr sp_Entry);
    void handleUnactiveConn(){timeManager_.click();}
    void setMysqlPool(Mysql_pool* mysqlPool);
<<<<<<< HEAD
=======
    void setRedisPool(CachePool* redisPool);
>>>>>>> 6d98924 (first common)
    void UpdateVoteSituation();
    void InitVoteSituation();
    void AddVote(int voteWhat,int option);
    int getOrder(){return loopOrder_;}
<<<<<<< HEAD
private:
    void handleRead(TimeStamp ts);
    void updateHtml();
=======
    void updateHtml();
private:
    void handleRead(TimeStamp ts);
>>>>>>> 6d98924 (first common)
    void doPendingFunctors(); 
    void abortNotInLoopThread();
    void voteInit();
    bool looping_;
    bool quit_;
    bool eventHandleing;
    bool callingPendingFunctors;
    int wakeupFd_;
    const pid_t threadId_;
    int loopOrder_;
    std::shared_ptr<Channel> wakeupChan_;
    std::shared_ptr<Epoll> poller_;
    std::vector<Functor> pendingFunctors_;
    MutexLock mutex_;
    TimeManager timeManager_;
    Mysql_pool* mysqlPool_;               //数据库池指针
<<<<<<< HEAD
=======
    CachePool* redisPool_;                //缓存数据库指针
>>>>>>> 6d98924 (first common)
    int voteAdd[2][4];                     //记录某一时间段的投票数
    int voteSituation[2][4];               //记录总票数
    int voteUpdateInterval;
};
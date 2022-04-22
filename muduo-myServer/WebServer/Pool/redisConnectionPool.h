#pragma once
#include<vector>
#include<memory>
#include<hiredis/hiredis.h>
#include<list>
#include"../syncTools/Mutex.h"

class CachePool{
public:
    CachePool();
    ~CachePool();
    static CachePool* getInstance();
    void init(std::string host,int port,int _capacity);
    redisContext* getConn();
    void recoveryConn(redisContext* redisConnPtr);
    void DestroyPool();
    
private:
    static CachePool* cachePoolptr;
    std::list<redisContext*> redisConnPool;
    int capacity;
    int freeConn;
    int curConn;
    int maxConn;
    sem reserve;
    std::string m_host;
    int m_port;
    MutexLock mutex_;
};
class redisRAII{
public:
    redisRAII(redisContext** ppRedis,CachePool* _cachePoolPtr);
    ~redisRAII();
private:
    CachePool* cachePoolPtr;
    redisContext* pRedis; 
};
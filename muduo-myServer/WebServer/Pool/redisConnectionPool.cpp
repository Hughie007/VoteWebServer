#include"../Log/logging.h"
#include "redisConnectionPool.h"
CachePool::CachePool(){
    freeConn=0;
    curConn=0;
    maxConn=0;
    capacity=0;
}
CachePool::~CachePool(){
}
CachePool* CachePool::cachePoolptr=nullptr;
CachePool* CachePool::getInstance(){
    if(cachePoolptr==nullptr){
        cachePoolptr=new CachePool();
    }
    return cachePoolptr;
}
void CachePool::init(std::string host,int port,int _capacity){
    m_host=host;
    m_port=port;
    for(int i=0;i<_capacity/2;i++){
        redisContext* c=redisConnect(m_host.c_str(),m_port);
        if(c==nullptr||c->err){
            if(c){
                LOG<<"Error:"+(std::string)c->errstr+"\n";
            }
            else{
                LOG<<"Cant't allocate redis context\n";
            }
            exit(1);
        }
        else{
            redisConnPool.push_back(c);
            freeConn++;
        }
    }
    for(int i=_capacity/2;i<_capacity;i++){
        redisContext* c=redisConnect(m_host.c_str(),m_port+1);
        if(c==nullptr||c->err){
            if(c){
                LOG<<"Error:"+(std::string)c->errstr+"\n";
            }
            else{
                LOG<<"Cant't allocate redis context\n";
            }
        }
        else{
            redisConnPool.push_back(c);
            freeConn++;
        }
    }
    reserve=sem(freeConn);
    maxConn=freeConn;
}
redisContext* CachePool::getConn(){
    redisContext* rs=nullptr;
    reserve.wait();
    {
        MutexLockGuard lock(mutex_);   
        rs=redisConnPool.front();
        redisConnPool.pop_front();
    }
    freeConn--;
    curConn++;
    return rs;
}
void CachePool::recoveryConn(redisContext* redisConnPtr){
    {
        MutexLockGuard lock(mutex_); 
        redisConnPool.push_back(redisConnPtr);
    }
    freeConn++;
    curConn--;
    reserve.post();
}
void CachePool::DestroyPool(){
    for(int i=0;i<maxConn;i++){
        redisContext* pRedis=redisConnPool.front();
        redisConnPool.pop_front();
        redisFree(pRedis);
    }
}
redisRAII::redisRAII(redisContext** _ppRedis,CachePool* _cachePoolPtr){
    cachePoolPtr=_cachePoolPtr;
    *_ppRedis=cachePoolPtr->getConn();
    pRedis=*_ppRedis;
}
redisRAII::~redisRAII(){
    cachePoolPtr->recoveryConn(pRedis);
}
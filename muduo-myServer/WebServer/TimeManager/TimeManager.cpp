#include"TimeManager.h"
#include"../TcpConnection/TcpConn.h"
TimeManager::TimeManager(int interval)
    :connectionTimeWheel_(){
        connectionTimeWheel_.resize(interval);
    }
TimeManager::~TimeManager()
    {}
void TimeManager::setInterval(int interval){
    connectionTimeWheel_.resize(interval);
}
void TimeManager::addTimer(EntryPtr sp_Entry){
    connectionTimeWheel_.back().insert(sp_Entry);
}
void TimeManager::click(){
    connectionTimeWheel_.push_back(Bucket());
}
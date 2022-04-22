#pragma once
#include"../varia.h"
#include"../Callback.h"

class TimeManager:public noncopyable{
private:
    TimeWheel connectionTimeWheel_;
public:
    TimeManager(int interval);
    ~TimeManager();
    void setInterval(int interval);
    void addTimer(EntryPtr sp_Entry);
    void click();
};
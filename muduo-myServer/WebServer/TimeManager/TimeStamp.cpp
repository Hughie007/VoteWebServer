#include<sys/time.h>
#include<inttypes.h>
#include"TimeStamp.h"

const int TimeStamp::kOneMillionMicroSecond_=1000000;
TimeStamp::TimeStamp()
    :microSecondSinceEpoch_(0)
    {}
TimeStamp::~TimeStamp()
    {}
TimeStamp TimeStamp::now(){
    struct timeval tv;
    gettimeofday(&tv,NULL);
    int64_t seconds=tv.tv_sec;
    return TimeStamp(seconds*kOneMillionMicroSecond_+tv.tv_usec);
}
TimeStamp::TimeStamp(int64_t microSecondArg)
    :microSecondSinceEpoch_(microSecondArg)
    {}
int64_t TimeStamp::getMicroSecondsSinceEpoch() const{
    return microSecondSinceEpoch_;
}
int64_t TimeStamp::getSecondsSinceEpoch() const{
    return static_cast<time_t>(microSecondSinceEpoch_/kOneMillionMicroSecond_);
}
void TimeStamp::swap(TimeStamp& ts){
    std::swap(microSecondSinceEpoch_,ts.microSecondSinceEpoch_);
}
bool TimeStamp::valid() const{
    return microSecondSinceEpoch_>0;
}
TimeStamp TimeStamp::fromUnixTime(time_t t){
    return fromUnixTime(t,0);
}
TimeStamp TimeStamp::fromUnixTime(time_t t,int microSeconds){
    return TimeStamp(static_cast<int64_t>(t)*kOneMillionMicroSecond_+microSeconds);
}
inline bool operator<(TimeStamp lts,TimeStamp rts){
    return lts.getMicroSecondsSinceEpoch()<rts.getMicroSecondsSinceEpoch();
}
inline bool operator==(TimeStamp lts,TimeStamp rts){
    return lts.getMicroSecondsSinceEpoch()==rts.getMicroSecondsSinceEpoch();
}
std::string TimeStamp::toString() const{
    char buf[32];
    int64_t seconds=microSecondSinceEpoch_/kOneMillionMicroSecond_;
    int64_t microSeconds=microSecondSinceEpoch_%kOneMillionMicroSecond_;
    snprintf(buf,sizeof buf,"%" PRId64 ".%06" PRId64,seconds,microSeconds);  //06表示输出6位，不够的话左边补0
    return buf;
}
std::string TimeStamp::toFormatString(bool showMicroSeconds) const{
    char buf[64]={0};
    time_t seconds=microSecondSinceEpoch_/kOneMillionMicroSecond_;
    struct tm time_tm;
    gmtime_r(&seconds,&time_tm);
    if(showMicroSeconds){
        int microSeconds=microSecondSinceEpoch_%kOneMillionMicroSecond_;
        snprintf(buf,sizeof buf,"%4d%02d%2d %02d:%2d:%2d.%06d",time_tm.tm_year+1900,time_tm.tm_mon+1,
                time_tm.tm_mday,time_tm.tm_hour,time_tm.tm_min,time_tm.tm_sec,microSeconds);
    }
    else{
        snprintf(buf,sizeof buf,"%4d%02d%2d %02d:%2d:%2d",time_tm.tm_year+1900,time_tm.tm_mon+1,
                time_tm.tm_mday,time_tm.tm_hour,time_tm.tm_min,time_tm.tm_sec);
    }
    return buf;
}
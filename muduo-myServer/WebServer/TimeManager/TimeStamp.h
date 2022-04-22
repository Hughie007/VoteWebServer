#pragma once
#include <boost/operators.hpp>
#include"../varia.h"
class TimeStamp:public copyable,
                public boost::equality_comparable<TimeStamp>,
                public boost::less_than_comparable<TimeStamp>{
public:
    TimeStamp();
    TimeStamp(int64_t t);
    ~TimeStamp();
    void swap(TimeStamp& ts);
    static TimeStamp fromUnixTime(time_t t);
    static TimeStamp fromUnixTime(time_t t,int microSeconds);
    std::string toString() const;
    std::string toFormatString(bool showMicroSeconds=true) const;
    bool valid() const;
    int64_t getMicroSecondsSinceEpoch() const;
    time_t getSecondsSinceEpoch() const;
    static TimeStamp now();
private:
    static const int kOneMillionMicroSecond_;
    int64_t microSecondSinceEpoch_;
};

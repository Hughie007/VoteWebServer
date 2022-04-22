#include<algorithm>
#include"LogStream.h"

template<typename T>
size_t convert(char buf[],T value){
    T i=value;
    char* cur=buf;
    while(i>0){
        int units=i%10;
        i/=10;
        *cur++=digits[units+9];
    }
    if(value<0){
        *cur++='-';
    }
    *cur='\0';
    std::reverse(buf,cur);
    return cur-buf;
}
template<typename T>
void LogStream::formatInteger(T v){
    if(buffer_.avail()>maxLeaveRoom){
        size_t len=convert(buffer_.current(),v);
        buffer_.add(len);
    }
}
LogStream& LogStream::operator<<(short v){
    *this<<static_cast<int>(v);
    return *this;
}
LogStream& LogStream::operator<<(unsigned short v){
    *this<<static_cast<int>(v);
    return *this;
}
LogStream& LogStream::operator<<(int v){
    formatInteger(v);
    return *this;
}
LogStream& LogStream::operator<<(unsigned int v){
    formatInteger(v);
    return *this;
}
LogStream& LogStream::operator<<(long v){
    formatInteger(v);
    return *this;
}
LogStream& LogStream::operator<<(unsigned long v){
    formatInteger(v);
    return *this;
}
LogStream& LogStream::operator<<(long long v){
    formatInteger(v);
    return *this;
}
LogStream& LogStream::operator<<(unsigned long long v){
    formatInteger(v);
    return *this;
}
LogStream& LogStream::operator<<(const char* cstr){
    if(cstr){
        buffer_.append(cstr,strlen(cstr));
    }
    else{
        buffer_.append("(null)",6);
    }
    return *this;
}
LogStream& LogStream::operator<<(double v){
    if(buffer_.avail()>maxLeaveRoom){
        int len=snprintf(buffer_.current(),maxLeaveRoom,"%.12g",v);
        buffer_.add(len);
    }
    return *this;
}
LogStream& LogStream::operator<<(long double v){
    if(buffer_.avail()>maxLeaveRoom){
        int len=snprintf(buffer_.current(),maxLeaveRoom,"%.12Lg",v);
        buffer_.add(len);
    }
    return *this;
}
LogStream& LogStream::operator<<(const std::string& str){
    buffer_.append(str.c_str(),str.size());
    return *this;
}
LogStream& LogStream::operator<<(bool v){
    buffer_.append(v?"1":"0",1);
    return *this;
}
void LogStream::append(const char* information,size_t len){
    buffer_.append(information,len);
}
void LogStream::resetBuffer(){
    buffer_.reset();
}
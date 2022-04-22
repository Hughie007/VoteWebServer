#include"Buffer.h"
#include"../Endian.h"

Buffer::Buffer(size_t kRoom)
    :buffer_(kRoom+kprepIndex),
     readIndex_(kprepIndex),
     writeIndex_(kprepIndex)
     {}
void Buffer::ensureWriteAble(size_t len){
    if(len>getWriteAble()){
        enlargeSpace(len);
    }
    assert(getWriteAble()>=len);
}
void Buffer::enlargeSpace(size_t len){
    if((getReadIndex()-kprepIndex+getWriteAble())>=len){
        size_t readAble=getReadAble();
        std::copy(begin()+readIndex_,getWriteLoc(),begin()+kprepIndex);
        retrieveAll();
        writeIndex_+=readAble;
    }
    else{
        buffer_.resize(writeIndex_+len);
    }
}
size_t Buffer::readFd(int fd,int* savedErro){
    char extraBuf[65536];
    struct iovec vec[2];
    const size_t writeAble=getWriteAble();
    vec[0].iov_base=const_cast<char*>(getWriteLoc());
    vec[0].iov_len=writeAble;
    vec[1].iov_base=extraBuf;
    vec[1].iov_len=sizeof(extraBuf);
    const ssize_t n=::readv(fd,vec,2);
    if(n<0){
        *savedErro=errno;
    }
<<<<<<< HEAD
    else if(implicit_cast<size_t>(n)<writeAble){      
=======
    else if(implicit_cast<size_t>(n)<writeAble){      //不懂为什么要implicit_cast而不用别的转换
>>>>>>> 6d98924 (first common)
        writeIndex_+=n;
    }
    else{
        writeIndex_=buffer_.size();
        append(extraBuf,n-writeIndex_);
    }
    return n;
}
char* Buffer::begin(){
    return &*buffer_.begin();
}
const char* Buffer::begin() const{
    return &*buffer_.begin();
}
const char* Buffer::getReadLoc() const{
    return begin()+readIndex_;
}
char* Buffer::getWriteLoc(){
    return begin()+writeIndex_;
}
const char* Buffer::getWriteLoc() const{
    return begin()+writeIndex_;
}
size_t Buffer::getReadIndex() const{
    return readIndex_;
}
size_t Buffer::getReadAble() const{
    return writeIndex_-readIndex_;
}
size_t Buffer::getWriteAble() const{
    return buffer_.size()-writeIndex_;
}
void Buffer::hasWritten(size_t len){
    assert(len<=getWriteAble());
    writeIndex_+=len;
}
void Buffer::retrieveAll(){
    readIndex_=kprepIndex;
    writeIndex_=kprepIndex;
}
void Buffer::retrieve(size_t len){
    assert(len<=getReadAble());
    if(len<getReadAble()){
        readIndex_+=len;
    }
    else{
        retrieveAll();
    }
}
void Buffer::retrieveUntil(const char* end){
    assert(end<=getWriteLoc());
    assert(end>=getReadLoc());
    retrieve(end-getReadLoc());
}
std::string Buffer::retrieveAsString(size_t len){
    assert(len<=getReadAble());
    std::string result(getReadLoc(),len);
    retrieve(len);
    return result;
}
std::string Buffer::retrieveAllString(){
    std::string result(getReadLoc(),getReadAble());
    retrieveAll();
    return result;
}
void Buffer::append(const char* data,size_t len){
    ensureWriteAble(len);
    std::copy(data,data+len,getWriteLoc());
    hasWritten(len);
}
void Buffer::append(const void* data,size_t len){
<<<<<<< HEAD
    append(static_cast<const char*>(data),len);    
=======
    append(static_cast<const char*>(data),len);    //具体是可以转化哪些类型呢？
>>>>>>> 6d98924 (first common)
}
void Buffer::appendIn64(int64_t v){
    int64_t val=muduo::net::sockets::hostToNetwork64(v);
    append(&val,sizeof val);
}
int64_t Buffer::readIn64(){
    assert(getReadAble()>sizeof(int64_t));
    int64_t val=0;
    ::memcpy(&val,getReadLoc(),sizeof(val));
    retrieve(sizeof(val));
    return val;
}
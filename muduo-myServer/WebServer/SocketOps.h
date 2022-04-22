#pragma once
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include<string.h>
#include<stdio.h>
#include<arpa/inet.h>
#include<assert.h>

inline void memZero(void* p, size_t n){
    memset(p, 0, n);
}
namespace muduo
{
namespace net
{
namespace sockets
{
    void close(int socket);
    void shotDownWrite(int socket);
    ssize_t write(int sockfd,const void* buf,size_t count);
    const struct sockaddr* sockaddr_cast(const struct sockaddr_in6* addr);
    struct sockaddr* sockaddr_cast(struct sockaddr_in6* addr);
    const struct sockaddr* sockaddr_cast(const struct sockaddr_in* addr);
    const struct sockaddr_in* sockaddr_in_cast(const struct sockaddr* addr);
    const struct sockaddr_in6* sockaddr_in6_cast(const struct sockaddr* addr);
    int createNonblockingOrDie(sa_family_t family);
    void bindOrDie(int sockfd, const struct sockaddr* addr);
    int accept(int sockfd, struct sockaddr_in6* addr);
    void toIp(char* buf, size_t size,const struct sockaddr* addr);
    void toIpPort(char* buf,size_t size,const struct sockaddr* addr);
    void listenOrDie(int sockfd);
    struct sockaddr_in6 getLocalAddr(int sockfd);
}
}
}
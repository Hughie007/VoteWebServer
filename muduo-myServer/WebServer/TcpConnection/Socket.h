#pragma once
#include<sys/socket.h>
#include <unistd.h>
#include<netinet/tcp.h>
#include"../varia.h"
class InetAddress;
class Socket:public noncopyable{
public:
    Socket(int sockfd);
    ~Socket();
    int fd() const;
    void shotDownWrite();
    void setReuseAddr(bool on);
    void bindAddress(const InetAddress& addr);
    void setReusePort(bool on);
    int accept(InetAddress* peeraddr);
    void listen();
    void setTcpNoDelay(bool on);
private:
    const int sockfd_;
};
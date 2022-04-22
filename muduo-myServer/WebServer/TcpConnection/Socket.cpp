#include"Socket.h"
#include"../SocketOps.h"
#include"../InetAddress.h"

using namespace muduo::net;
Socket::Socket(int sockfd)
    :sockfd_(sockfd)
    {}
Socket::~Socket(){
    sockets::close(sockfd_);
}
void Socket::shotDownWrite(){
    sockets::shotDownWrite(sockfd_);
}
int Socket::fd() const{
    return sockfd_;
}
void Socket::setReuseAddr(bool on){
    int optval = on ? 1 : 0;
    ::setsockopt(sockfd_, SOL_SOCKET, SO_REUSEADDR,
               &optval, static_cast<socklen_t>(sizeof optval));
}
void Socket::bindAddress(const InetAddress& addr){
    muduo::net::sockets::bindOrDie(sockfd_, addr.getSockAddr());
}
void Socket::setReusePort(bool on){
#ifdef SO_REUSEPORT
    int optval = on ? 1 : 0;
    int ret = ::setsockopt(sockfd_, SOL_SOCKET, SO_REUSEPORT,
                            &optval, static_cast<socklen_t>(sizeof optval));
    if (ret < 0 && on){
        //LOG_SYSERR << "SO_REUSEPORT failed.";
    }
#else
    if (on){
        LOG_ERROR << "SO_REUSEPORT is not supported.";
    }
#endif
}
int Socket::accept(InetAddress* peeraddr)
{
  struct sockaddr_in6 addr;
  memZero(&addr, sizeof addr);
  int connfd = sockets::accept(sockfd_, &addr);
  if (connfd >= 0)
  {
    peeraddr->setSockAddrInet6(addr);
  }
  return connfd;
}
void Socket::listen(){
    muduo::net::sockets::listenOrDie(sockfd_);
}
void Socket::setTcpNoDelay(bool on){
    int optval=on?1:0;
    ::setsockopt(sockfd_,IPPROTO_TCP,TCP_NODELAY,&optval,sizeof optval);
}
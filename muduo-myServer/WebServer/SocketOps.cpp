#include <errno.h>
#include"SocketOps.h"
#include"Endian.h"
#include"./Log/logging.h"
using namespace muduo::net;
void sockets::close(int sockfd){
    if(::close(sockfd)<0){
        //Log...
    }
}
void sockets::shotDownWrite(int sockfd){           //会触发读事件
    if(::shutdown(sockfd,SHUT_WR)<0){
        //Log...
    }
}
ssize_t sockets::write(int sockfd,const void* buf,size_t count){
    return ::write(sockfd,buf,count);
}
const struct sockaddr* sockets::sockaddr_cast(const struct sockaddr_in6* addr){
    return static_cast<const struct sockaddr*>(implicit_cast<const void*>(addr));
}
struct sockaddr* sockets::sockaddr_cast(struct sockaddr_in6* addr){
    return static_cast<struct sockaddr*>(implicit_cast<void*>(addr));
}
int sockets::createNonblockingOrDie(sa_family_t family){
#if VALGRIND                       //干啥的?
    int sockfd = ::socket(family, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd < 0)
    {
        LOG_SYSFATAL << "sockets::createNonblockingOrDie";
    }

    setNonBlockAndCloseOnExec(sockfd);
#else
    int sockfd = ::socket(family, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP);   //分别表示TCP字节流服务，非阻塞，一旦进程exec新程序则关闭fd,TCP协议
    if (sockfd < 0){
    //LOG_SYSFATAL << "sockets::createNonblockingOrDie";
    }
#endif
    return sockfd;
}
void sockets::bindOrDie(int sockfd, const struct sockaddr* addr){
    int ret = ::bind(sockfd, addr, static_cast<socklen_t>(sizeof(struct sockaddr_in6)));
    if (ret < 0){
    //LOG_SYSFATAL << "sockets::bindOrDie";
    }
}

int sockets::accept(int sockfd, struct sockaddr_in6* addr)
{
    socklen_t addrlen = static_cast<socklen_t>(sizeof *addr);
#if VALGRIND || defined (NO_ACCEPT4)
    int connfd = ::accept(sockfd, sockaddr_cast(addr), &addrlen);
    setNonBlockAndCloseOnExec(connfd);
#else
    int connfd = ::accept4(sockfd, sockaddr_cast(addr),
                         &addrlen, SOCK_NONBLOCK | SOCK_CLOEXEC);
#endif
    if (connfd < 0){
        int savedErrno = errno;
        //LOG_SYSERR << "Socket::accept";
        switch (savedErrno)
        {
            case EAGAIN:
            case ECONNABORTED:
            case EINTR:
            case EPROTO: // ???
            case EPERM:
            case EMFILE: // per-process lmit of open file desctiptor ???
                // expected errors
                errno = savedErrno;
                break;
            case EBADF:
            case EFAULT:
            case EINVAL:
            case ENFILE:
            case ENOBUFS:
            case ENOMEM:
            case ENOTSOCK:
            case EOPNOTSUPP:
                    // unexpected errors
                //LOG_FATAL << "unexpected error of ::accept " << savedErrno;
                break;
            default:
                //LOG_FATAL << "unknown error of ::accept " << savedErrno;
                break;
        }
    }
    return connfd;
}
const struct sockaddr* sockets::sockaddr_cast(const struct sockaddr_in* addr){
    return static_cast<const struct sockaddr*>(implicit_cast<const void*>(addr));
}
const struct sockaddr_in* sockets::sockaddr_in_cast(const struct sockaddr* addr){
    return static_cast<const struct sockaddr_in*>(implicit_cast<const void*>(addr));
}
const struct sockaddr_in6* sockets::sockaddr_in6_cast(const struct sockaddr* addr){
    return static_cast<const struct sockaddr_in6*>(implicit_cast<const void*>(addr));
}
void sockets::toIp(char* buf, size_t size,
                   const struct sockaddr* addr){
    if (addr->sa_family == AF_INET)
    {
        assert(size >= INET_ADDRSTRLEN);
        const struct sockaddr_in* addr4 = sockets::sockaddr_in_cast(addr);
        ::inet_ntop(AF_INET, &addr4->sin_addr, buf, static_cast<socklen_t>(size));
    }
    else if (addr->sa_family == AF_INET6)
    {
        assert(size >= INET6_ADDRSTRLEN);
        const struct sockaddr_in6* addr6 = sockets::sockaddr_in6_cast(addr);
        ::inet_ntop(AF_INET6, &addr6->sin6_addr, buf, static_cast<socklen_t>(size));
    }
}
void sockets::toIpPort(char* buf,size_t size,const struct sockaddr* addr){
    if(addr->sa_family == AF_INET6){

    }
    toIp(buf,size,addr);
    size_t end=::strlen(buf);
    const struct sockaddr_in* addr4= muduo::net::sockets::sockaddr_in_cast(addr);
    uint16_t port= muduo::net::sockets::networkToHost16(addr4->sin_port);
    assert(size>end);
    snprintf(buf+end,size-end,":%u",port);
}
void sockets::listenOrDie(int sockfd){
    int ret=::listen(sockfd,SOMAXCONN);
    if(ret<0){
        //LOG...
    }
}
struct sockaddr_in6 sockets::getLocalAddr(int sockfd){
    struct sockaddr_in6 localaddr;
    memZero(&localaddr,sizeof localaddr);
    socklen_t addrlen=static_cast<socklen_t>(sizeof localaddr);
    if(::getsockname(sockfd,sockaddr_cast(&localaddr),&addrlen)){   //本地地址将被返回
        LOG<<"sockets::getLocalAddr";
    }
    return localaddr;
}
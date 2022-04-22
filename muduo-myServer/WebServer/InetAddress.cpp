#include<string.h>
#include"SocketOps.h"
#include"InetAddress.h"
#include"Endian.h"
static const in_addr_t kInaddrAny = INADDR_ANY;                //0.0.0.0 本机的任意一块网卡
static const in_addr_t kInaddrLoopback = INADDR_LOOPBACK;      //127.0.0.1 
InetAddress::InetAddress(uint16_t portArg, bool loopbackOnly, bool ipv6){
    static_assert(offsetof(InetAddress, addr6_) == 0, "addr6_ offset 0");    //static_assert在编译期间断言，offsetof返回成员在结构中的便宜量
    static_assert(offsetof(InetAddress, addr_) == 0, "addr_ offset 0");
    if (ipv6)
    {
        memset(&addr6_,0,sizeof addr6_);
        addr6_.sin6_family = AF_INET6;
        in6_addr ip = loopbackOnly ? in6addr_loopback : in6addr_any;
        addr6_.sin6_addr = ip;
        addr6_.sin6_port = muduo::net::sockets::hostToNetwork16(portArg);
    }
    else
    {
        memset(&addr_,0,sizeof addr_);
        addr_.sin_family = AF_INET;
        in_addr_t ip = loopbackOnly ? kInaddrLoopback : kInaddrAny;
        addr_.sin_addr.s_addr = muduo::net::sockets::hostToNetwork32(ip);    //主机序转网络序号，小端转大端
        addr_.sin_port = muduo::net::sockets::hostToNetwork16(portArg);
    }
}
InetAddress::InetAddress(const struct sockaddr_in6& addr6)
    :addr6_(addr6)
    {}
InetAddress::InetAddress(const struct sockaddr_in& addr)
    :addr_(addr)
    {}
sa_family_t InetAddress::family() const {
    return addr_.sin_family;
}
std::string InetAddress::toIp() const{
    char buf[64]="";
    muduo::net::sockets::toIp(buf,sizeof buf,getSockAddr());
    return buf;
}
std::string InetAddress::toIpPort() const{
    char buf[64]="";
    muduo::net::sockets::toIpPort(buf,sizeof buf,getSockAddr());
    return buf;
}
const struct sockaddr* InetAddress::getSockAddr() const { 
    return muduo::net::sockets::sockaddr_cast(&addr6_);
}
void InetAddress::setSockAddrInet6(const struct sockaddr_in6& addr6) {
    addr6_ = addr6;
}
uint16_t InetAddress::portNetEndian() const {
    return addr_.sin_port;
}
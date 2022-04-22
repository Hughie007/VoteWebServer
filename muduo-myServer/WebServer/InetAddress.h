#pragma once
#include <netinet/in.h>
#include<string>
#include"varia.h"
#include"SocketOps.h"

class InetAddress : public copyable{
public:
    explicit InetAddress(uint16_t port = 0, bool loopbackOnly = false, bool ipv6 = false);
    explicit InetAddress(const struct sockaddr_in6& addr6);
    explicit InetAddress(const struct sockaddr_in& addr);
    //InetAddress(StringArg ip, uint16_t port, bool ipv6 = false);
    //explicit InetAddress(const struct sockaddr_in& addr)
    //:addr_(addr)
    //{}

    //explicit InetAddress(const struct sockaddr_in6& addr)
    //:addr6_(addr)
    //{}

    sa_family_t family() const;
    std::string toIp() const;
    std::string toIpPort() const;


    const struct sockaddr* getSockAddr() const;
    void setSockAddrInet6(const struct sockaddr_in6& addr6);

    uint32_t ipv4NetEndian() const;
    uint16_t portNetEndian() const;

    static bool resolve(StringArg hostname, InetAddress* result);

    void setScopeId(uint32_t scope_id);

private:
    union
    {
        struct sockaddr_in addr_;
        struct sockaddr_in6 addr6_;
    };
};
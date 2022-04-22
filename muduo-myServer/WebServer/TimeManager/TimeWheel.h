#pragma once
#include<memory>
class TcpConn;

struct Entry{
public:
    typedef std::weak_ptr<TcpConn> wp_TcpConn;
    explicit Entry(const wp_TcpConn& weakConn);
    ~Entry();
    wp_TcpConn weakConn_;
};
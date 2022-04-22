#include"TimeWheel.h"
#include"../TcpConnection/TcpConn.h"
Entry::Entry(const wp_TcpConn& weakConn)
    :weakConn_(weakConn)
    {}
Entry::~Entry(){
    std::shared_ptr<TcpConn> conn_=weakConn_.lock();
    if(conn_){
        conn_->handleClose();
    }
};
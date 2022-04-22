#include<string>
#include <mysql/mysql.h>
#include<list>
#include"../varia.h"
#include"../syncTools/Mutex.h"
class Mysql_pool:public noncopyable{
private:
    Mysql_pool();         //单例模式
    Mysql_pool(Mysql_pool&)=delete;
    Mysql_pool& operator=(const Mysql_pool&)=delete;
    ~Mysql_pool(){};        //单例模式
    static Mysql_pool* dataBasePool_;
    int m_MaxConn;             //最大连接数
    int m_CurConn;             //当前连接数
    int m_FreeConn;            //空闲连接数
    MutexLock mutex_;
    std::list<MYSQL*> connList;  //连接池
    sem reserve;
public:
    static Mysql_pool* getInstance();
    void init(std::string url,std::string User,std::string PassWord,std::string DBName,int port,int MaxConn);
    MYSQL* getMysqlConn();        //获取数据库连接
    bool ReleaseMysql(MYSQL* mysql);
    void DestroyPool();

public:
	std::string m_url;			 //主机地址
	std::string m_Port;		 //数据库端口号
	std::string m_User;		 //登陆数据库用户名
	std::string m_PassWord;	 //登陆数据库密码
	std::string m_DatabaseName; //使用数据库名
};
class MysqlRAII{
private:
    MYSQL* mysqlRAII_;                    //记录连接以及连接池以便析构时将链接返还给连接池
    Mysql_pool* poolRAII_;
public:
    MysqlRAII(MYSQL **con,Mysql_pool* mysqlPool);
    ~MysqlRAII();
};
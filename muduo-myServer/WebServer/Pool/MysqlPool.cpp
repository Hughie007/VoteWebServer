#include"MysqlPool.h"
#include"../Log/logging.h"
Mysql_pool* Mysql_pool::dataBasePool_=nullptr;
Mysql_pool::Mysql_pool(){
	m_CurConn = 0;
	m_FreeConn = 0;
}
MYSQL* Mysql_pool::getMysqlConn(){
	MYSQL *con = NULL;
    
    if (0 == connList.size())
        return NULL;

	reserve.wait();
	
	{
        MutexLockGuard lock(mutex_);          //共享资源要加锁
        con = connList.front();
        connList.pop_front();

        --m_FreeConn;
        ++m_CurConn;

    }
	return con;
}
bool Mysql_pool::ReleaseMysql(MYSQL *con){
	if (NULL == con)
		return false;

	{
        MutexLockGuard lock(mutex_);
        connList.push_back(con);
        ++m_FreeConn;
        --m_CurConn;
    }

	reserve.post();
	return true;
}
Mysql_pool* Mysql_pool::getInstance(){
    if(dataBasePool_==nullptr){
        dataBasePool_=new Mysql_pool();
    }
    return dataBasePool_;
}
void Mysql_pool::init(std::string url,std::string User,std::string PassWord,std::string DBName,int port,int MaxConn){
    m_url=url;
    m_Port=port;
    m_User=User;
    m_PassWord=PassWord;
    m_DatabaseName=DBName;
    for (int i = 0; i < MaxConn; i++){
		MYSQL *con = NULL;
		con = mysql_init(con);

		if (con == NULL)
		{
			LOG<<"MySQL Error";
			exit(1);
		}
		con = mysql_real_connect(con, url.c_str(), User.c_str(), PassWord.c_str(), DBName.c_str(), port, NULL, 0);

		if (con == NULL)
		{
			//std::cout<<"erere"<<endl;
			LOG<<"MySQL Error";  
			//std::cout<<mysql_error(con);
			exit(1);
		}
		//std::cout<<"success"<<endl;
		connList.push_back(con);
		++m_FreeConn;
	}

	reserve = sem(m_FreeConn);

	m_MaxConn = m_FreeConn;
}
void Mysql_pool::DestroyPool(){            //销毁数据库连接池

	{
        MutexLockGuard lock(mutex_);           //临界区加锁
        if (connList.size() > 0){
            std::list<MYSQL *>::iterator it;
            for(it = connList.begin(); it != connList.end(); ++it){
                MYSQL *con = *it;
                mysql_close(con);
            }
            m_CurConn = 0;
            m_FreeConn = 0;
            connList.clear();
        }
	}
}
MysqlRAII::MysqlRAII(MYSQL **con,Mysql_pool* mysqlPool){
    *con=mysqlPool->getMysqlConn();
    mysqlRAII_=*con;
    poolRAII_=mysqlPool;
}
MysqlRAII::~MysqlRAII(){
    poolRAII_->ReleaseMysql(mysqlRAII_);       //析构时返回连接给连接池
}
#include"../Log/logging.h"
#include"dbSYN.h"
#include"../Pool/MysqlPool.h"
dbSYN::dbSYN(int _interval)
    :interval(_interval),
     redisConn(nullptr),
     mysqlConn(nullptr),
     thread_(std::bind(&dbSYN::synFuncLast,this)){
         for(int i=0;i<2;i++){
             for(int j=0;j<4;j++){
                 voteCount[i][j]=0;
             }
         }
    }
dbSYN::~dbSYN(){
    mysql_close(mysqlConn);
    redisFree(redisConn);
}
void dbSYN::setConn(){
    mysqlConn = mysql_init(mysqlConn);
    if (mysqlConn == NULL){
        LOG<<"MySQL Error";
        exit(1);
    }
    mysqlConn=mysql_real_connect(mysqlConn,"localhost", "root", "123456", "VoteSituation", 3306, NULL, 0);
    if (mysqlConn == NULL){
        LOG<<"MySQL Error";  
        exit(1);
	}
    redisConn=redisConnect("127.0.0.1",6379);
    if (redisConn == NULL||redisConn->err){
        LOG<<"Redis Error";  
        exit(1);
    }
}
void dbSYN::synFuncOnce(){
    if(mysql_query(mysqlConn,"SELECT option1,option2,option3,option4 FROM user")){
        LOG<<"erron on connection to mysql";
    }
    MYSQL_RES *result=mysql_store_result(mysqlConn);
    MYSQL_FIELD *fields=mysql_fetch_fields(result);
    for(int i=0;i<2;i++){
        MYSQL_ROW row=mysql_fetch_row(result);
        for(int j=0;j<4;j++){
            voteCount[i][j]=atoi(row[j]);
        }
    }
    redisReply * pReply = (redisReply*)redisCommand(redisConn, "mset vote00 %d vote01 %d vote02 %d vote03 %d \
                                                                    vote10 %d vote11 %d vote12 %d vote13 %d",\
                                                                    voteCount[0][0],voteCount[0][1],voteCount[0][2],voteCount[0][3],\
                                                                    voteCount[1][0],voteCount[1][1],voteCount[1][2],voteCount[1][3]);
    freeReplyObject( pReply );
}
void dbSYN::setExitLabel(){
    exitLabel=true;
}
bool dbSYN::getExitLabel(){
    return exitLabel;
}
void dbSYN::synFuncLast(){
    synFuncOnce();
    while(!exitLabel){
        {
            std::unique_lock<std::mutex> locker(mutex_);
            cond_.wait_for(locker,std::chrono::milliseconds(interval),[this](){return getExitLabel();});
        }
        synFuncOnce();
    }
}
void dbSYN::startSyn(){
    thread_.start();
}
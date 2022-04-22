#include "FileUtil.h"
<<<<<<< HEAD
AppendFile::AppendFile(std::string filename):fp_(fopen(filename.c_str(),"ae")){     
=======
AppendFile::AppendFile(std::string filename):fp_(fopen(filename.c_str(),"ae")){     //"ae"是什么
>>>>>>> 6d98924 (first common)
    setbuffer(fp_,buffer_,sizeof(buffer_));
}
AppendFile::~AppendFile(){
    fclose(fp_);
}
void AppendFile::append(const char* information,size_t len){
    size_t n=write(information,len);
    size_t remain=len-n;
    while(remain>0){
        size_t x=write(information+n,remain);
        if(x==0){
            int err=ferror(fp_);                          //返回非0值表示出错
            if(err) fprintf(stderr,"AppendFile::append() failed!\n");
            break;
        }
        n+=x;
        remain-=x;
    }
}
<<<<<<< HEAD
void AppendFile::flush(){fflush(fp_);}     
size_t AppendFile::write(const char* information,size_t len){
    return fwrite_unlocked(information,1,len,fp_);               
=======
void AppendFile::flush(){fflush(fp_);}     //刷新fp_的输出缓冲区，有啥用？
size_t AppendFile::write(const char* information,size_t len){
    return fwrite_unlocked(information,1,len,fp_);               //各个参数意义不太懂
>>>>>>> 6d98924 (first common)
}
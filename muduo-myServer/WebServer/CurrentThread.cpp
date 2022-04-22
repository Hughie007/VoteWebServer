#include"CurrentThread.h"
namespace CurrentThread {
// internal
__thread int t_cachedTid;                 //__thread可以使每个线程有单独的一份变量实例
__thread char t_tidString[32];
__thread int t_tidStringLength;
__thread const char* t_threadName;
void cacheTid() {
  if (t_cachedTid == 0) {
    t_cachedTid = gettid();
    t_tidStringLength =
        snprintf(t_tidString, sizeof t_tidString, "%5d ", t_cachedTid);  //%5d表示按5位固定位宽输出
  }
}
}
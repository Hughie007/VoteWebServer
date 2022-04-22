#pragma once
#include<string>
#include<sys/unistd.h>

namespace CurrentThread {
// internal
extern __thread int t_cachedTid;                 //__thread可以使每个线程有单独的一份变量实例
extern __thread char t_tidString[32];
extern __thread int t_tidStringLength;
extern __thread const char* t_threadName;
extern void cacheTid();
inline int tid() {                                        //Hb:内联函数一般在头文件定义
  if (__builtin_expect(t_cachedTid == 0, 0)) {           //提高效率，表示if()内部很可能执行，不过返回是相等还是不相等呢
    cacheTid();
  }
  return t_cachedTid;
}
}
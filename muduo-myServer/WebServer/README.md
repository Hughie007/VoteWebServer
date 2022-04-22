VoteWebServer
===============
Linux下基于muduo风格的C++投票Web服务器

* 使用 事件处理模型（Reactor）+ 多线程 + epoll实现的半同步半异步高并发模型
* 使用**数据库连接池**记录客户投票情况，并实时更新服务器票数展示网页
* 使用**状态模式**解析HTTP请求报文，支持解析**GET和POST**请求
* 实现**异步日志系统**，记录服务器运行状态
* 经Webbench压力测试可以实现**上万的并发连接**数据交换

写在前面
----
* 本项目记录自己后台开发的学习过程，考上研究生后自研一起开始学习C++相关知识，先后翻阅《C++ primer》，《大话数据结构》，《计算机网络》，《深入理解计算机系统》等经典书籍后开始刷题。平时在实验室也会帮导师做做项目，但是项目都是与视觉算法相关，因此打算自己在网上学习开源项目。研二上的时候经同学推荐，购买了《Linux多线程服务端编程》，接触并学习了其中陈硕大佬muduo网络库的实现思想与方法，于是借鉴其实现方法编写自己的第二个后台服务器。

* 第一次上传项目至Github，有什么不好的地方请指正，感谢各位学者。

目录
-----

| [概述](#概述) | [框架](#框架) | [Demo演示](#Demo演示) | [压力测试](#压力测试) |[更新日志](#更新日志) |[源码下载](#源码下载) | [快速运行](#快速运行) | [个性化运行](#个性化运行) | [庖丁解牛](#庖丁解牛) | [CPP11实现](#CPP11实现) |[致谢](#致谢) |
|:--------:|:--------:|:--------:|:--------:|:--------:|:--------:|:--------:|:--------:|:--------:|:--------:|:--------:|



概述
----------

> * C/C++
> * B/S模型
> * [反应堆包装类](https://github.com/qinguoyi/TinyWebServer/tree/master/lock)
> * [http连接请求处理类](https://github.com/qinguoyi/TinyWebServer/tree/master/http)
> * [半同步/半反应堆线程池](https://github.com/qinguoyi/TinyWebServer/tree/master/threadpool)
> * [定时器处理非活动连接](https://github.com/qinguoyi/TinyWebServer/tree/master/timer)
> * [同步/异步日志系统 ](https://github.com/qinguoyi/TinyWebServer/tree/master/log)  
> * [数据库连接池](https://github.com/qinguoyi/TinyWebServer/tree/master/CGImysql) 
> * [同步线程注册和登录校验](https://github.com/qinguoyi/TinyWebServer/tree/master/CGImysql) 
> * [简易服务器压力测试](https://github.com/qinguoyi/TinyWebServer/tree/master/test_presure)
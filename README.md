VoteWebServer
===============
Linux下基于muduo风格的C++投票Web服务器

* 使用 事件处理模型（Reactor）+ 多线程 + epoll实现的半同步半异步高并发模型
* 使用**数据库连接池**记录客户投票情况，并实时更新服务器票数展示网页
* 使用**状态模式**解析HTTP请求报文，支持解析**GET和POST**请求
* 实现**异步日志系统**，记录服务器运行状态
* 经Webbench压力测试可以实现**上万的并发连接**数据交换
* 编写redis数据库连接池和数据库同步工具，实现redis缓存数据库和mysql配合使用（2022-04添加）

写在前面
----
* 本项目记录自己后台开发的学习过程，考上研究生后自研一起开始学习C++相关知识，先后翻阅《C++ primer》，《大话数据结构》，《计算机网络》，《深入理解计算机系统》等经典书籍后开始刷题。平时在实验室也会帮导师做做项目，但是项目都是与视觉算法相关，因此打算自己在网上学习开源项目。研二上的时候经同学推荐，购买了《Linux多线程服务端编程》，接触并学习了其中陈硕大佬muduo网络库的实现思想与方法，于是借鉴其实现方法编写自己的第二个后台服务器。

* 第一次上传项目至Github，有什么不好的地方请指正，感谢各位学者。

目录
-----

| [概述](#概述) | [Demo演示](#Demo演示) | [压力测试](#压力测试) |[项目环境及运行方式](#项目环境及运行方式) |



概述
----------

> * C/C++
> * B/S模型
> * [反应堆包装类](https://github.com/Hughie007/VoteWebServer/tree/master/muduo-myServer/WebServer/Reactor)
> * [客户端服务器端Tcp连接处理类](https://github.com/Hughie007/VoteWebServer/tree/master/muduo-myServer/WebServer/TcpConnection)
> * [非活跃连接定时器处理类](https://github.com/Hughie007/VoteWebServer/tree/master/muduo-myServer/WebServer/TimeManager)
> * [线程池数据库池处理类](https://github.com/Hughie007/VoteWebServer/tree/master/muduo-myServer/WebServer/Pool)
> * [异步日志系统处理类](https://github.com/Hughie007/VoteWebServer/tree/master/muduo-myServer/WebServer/Log)
> * [异步日志系统处理类](https://github.com/Hughie007/VoteWebServer/tree/master/muduo-myServer/WebServer/Log)
> * [数据库同步工具类](https://github.com/Hughie007/VoteWebServer/tree/master/muduo-myServer/WebServer/DbSyn)
> * [前段展示网页](https://github.com/Hughie007/VoteWebServer/tree/master/muduo-myServer/WebServer/root) 
> * [服务器压力测试](https://github.com/Hughie007/VoteWebServer/tree/master/muduo-myServer/WebBench)




Demo演示
----------
> * 投票演示

<div align=center><img src="https://github.com/Hughie007/VoteWebServer/blob/master/resource/vote.gif" height="429"/> </div>




> * 查看投票情况演示

<div align=center><img src="https://github.com/Hughie007/VoteWebServer/blob/master/resource/voteSituation.gif" height="429"/> </div>






=======

压力测试
-------------
使用Webbench对服务器进行压力测试，测试结果如下图

> * LT + 非阻塞 + mysql


<div align=center><img src="https://github.com/Hughie007/VoteWebServer/blob/master/resource/pressTest0.jpg" height="201"/> </div>



> * 并发连接总数：4000
> * 访问服务器时间：10s
> * 所有访问均成功


-------------
2022-04
> 4月份利用空余时间学习了解redis，在项目基础之上添加了redis缓存连接池和数据库同步工具
>数据记录逻辑：
>    写入数据：直接写入mysql
>    读入数据：从redis集群中的从机读取
>    数据同步：开启新线程，每隔一段时间完成mysql数据到redis主机的同步
    
使用Webbench对服务器进行压力测试，测试结果如下图

> * LT + 非阻塞 + mysql + redis

<div align=center><img src="https://github.com/Hughie007/VoteWebServer/blob/master/resource/pressTest1.jpg" height="201"/> </div>



> * 并发连接总数：4000
> * 访问服务器时间：10s
> * 所有访问均成功

项目环境及运行方式
------------
* 服务器测试环境
	* Ubuntu版本21.04
	* MySQL版本8.0.27
* 浏览器测试环境
	* Windows、Linux均可
	* Chrome
	* FireFox
	* 其他浏览器暂无测试

* 测试前确认已安装MySQL数据库

    ```C++
    // 建立yourdb库
    create database yourdb;

    // 创建user表
    USE yourdb;
    CREATE TABLE user(
        voteWhat char(50) NULL,
        option1 int,
        option2 int,
        option3 int,
        option4 int
    )ENGINE=InnoDB;

    // 添加数据
    INSERT INTO user(voteWhat,option1,option2,option3,option4) VALUES('sport',0,0,0,0);
    INSERT INTO user(voteWhat,option1,option2,option3,option4) VALUES('fruit',0,0,0,0);


    * 修改TcpServer.cpp中的数据库初始化信息
    ```C++
    //数据库登录名,密码,库名
    #Mysql_pool::init(std::string url,std::string User,std::string PassWord,std::string DBName,int port,int MaxConn)
    #修改DBName为你定义的数据库名
    #修改MaxConn为数据库池大小
    ```

* build

    ```C++
    cd muduo-myServer
    ./build.sh
    ```

* 启动server

    ```C++
    cd ../build/Debug/WebServer
    ./myVoteServer
    ```
(直接makefile或是利用vscode得到的可执行文件并发数更高)

* 浏览器端

    ```C++
    ip:9981

    ```
=======
    ```




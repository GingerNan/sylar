# sylar

## 开发环境
Ubuntu
gcc
cmake

## 项目路径
bind -- 二进制文件
build -- 中间文件路径
cmake -- cmake函数文件夹
CMakeLists.txt -- cmake文件
lib -- 库的输出路径
Makefile
sylar -- 源代码路径
tests -- 测试代码
``
## 日志系统
1)
    Log4J

    Logger（定义日志类别）
        |
        |-------Formatter（日志格式）
        |
    Appender（日志输出地方）

## 配置系统

Config --> Yaml

- boost库：sudo apt install libboost-all-dev
- yamp-cpp: sudo apt install libyaml-cpp-dev


配置系统的原则，约定优于配置
```cpp
template<T, FormStr, ToStr>
class ConfigVar;

template<F, T>
LexicalCast;

//容器特例化，目前支持 vector、list、set、map、unordered_map、unordered_set
// Config::Lookup(key)，key相同，类型不同的，不会由报错

```

自定义类型，需要实现sylar::LexicalCast，特例化实现后，可以支持Config解析
自定义类型，自定义类型可以和常规stl容器一起使用

配置的事件机制 当一个配置项发生修改的时候，可以反向通知对应的代码，回调

## 日志系统整合配置系统
```yaml
logs:
    - name: root
      level: (debug,info,warn,error,fatal)
      formatter: '%d%T%p%T%t%m%n'
      appender:
        - type: (StdoutLogAppender, FileLogAppender)
          level: (debug,...)
          file: /logs/xxx.log

```
```cpp
    sylar::Logger g_logger = sylar::LoggerMg::GetInstance(name);
    SYLAR_LOG_INFO(g_logger) << "xxxx log";
```
## 协程库封装

定义协程接口 ucontext_t.
macro

```
Thread->main_fiber <------> sub_fiber
            |
            |
            v
        sub_fiber
```

协程调度模块scheduler
```
        1 - N      1 - M
scheduler --> thread --> fiber
1. 线程池，分配一组线程
2. 协程调度器，将协程，指定到相应的线程上去执行

N : M

m_threads
<function<void()>, fiber, threadid> m_fibers;

schedule(func/fiber)

start()
stop()
run()

1.设置当前线程的scheduler
2.设置当前线程的run，fiber
3.协程调度循环while(true)
    1.协程消息队列里面是否有任务
    2.无任务执行，执行idle
```

```
Timer -> addTimer() --> cancel()
获取当前的定时器触发离现在的时间差
返回当前需要触发的定时器
```

```
        [Fiber]                    [Timer]
            ^N                        ^
            |                         |
            |1                        |
        [Thread]                [TimerManager]
            ^M                        ^
            |                         |
            |1                        |
        [scheduler] <-------- [IOManager(epoll)]
```

## HOOK
sleep,
usleep

## socket函数库
```
        [UnixAddress]
              |
         -----------                    | - [IPv4Address]
          |Address| --- [IPAddress] --- |
         -----------                    | - [IPv6Address]
              |
              |
         -----------
          | Socket |
         -----------
```
connect, accept, read/write

## 序列化bytearray

write(int, float, int64, ...)
read(int, floag, int64, ...)

## http协议开发

HTTP/1.1 - API

HttpRequest;
HttpResponse;

GET / HTTP/1.1
host: www.baidu.com

HTTP/1.1 200 OK
Accept-Ranges: bytes
Cache-Control: no-cache
Connection: keep-alive
Content-Length: 17143

uri: http://www.baidu.com/page/xxx?id=10&20#fr
    http    协议
    www.sylar.top host
    80      端口
    /page/xxx path
    id=10&v=20 param
    fr fragment

## 分布协议

## 推荐系统

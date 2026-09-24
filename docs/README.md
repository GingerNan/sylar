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

## 协程库封装

## socket函数库

## http协议开发

## 分布协议

## 推荐系统

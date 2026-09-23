#include <iostream>
#include <thread>

#include "../sylar/log.h"
#include "../sylar/util.h"

int main(int argc, char** argv)
{
    sylar::Logger::ptr logger(new sylar::Logger);
    logger->addAppender(sylar::LogAppender::ptr(new sylar::StdoutLogAppender));

    sylar::FileLogAppender::ptr file_appender(new sylar::FileLogAppender("./log.txt"));
    logger->addAppender(file_appender);
    // 为文件Appender 设置不一样的日志输出格式
    sylar::LogFormatter::ptr fmt(new sylar::LogFormatter("%d%T[%p]%T%m%n"));
    file_appender->setFormatter(fmt);
    file_appender->setLevel(sylar::LogLevel::ERROR);


    // sylar::LogEvent::ptr event(new sylar::LogEvent(__FILE__, __LINE__, 0, sylar::GetThreadId(), sylar::GetFiberId(), time(0)));
    // logger->log(sylar::LogLevel::DEBUG, event);

    SYLAR_LOG_DEBUG(logger) << "test macro";
    SYLAR_LOG_ERROR(logger) << "test macor error";

    SYLAR_LOG_FMT_DEBUG(logger, "str:%s, val=%d", "test macro", 2);
    SYLAR_LOG_FMT_ERROR(logger, "str:%s, val=%d", "test macro", 2);

    auto l = sylar::LoggerMgr::GetInstance()->getLogger("xx");
    SYLAR_LOG_INFO(l) << "xxx";
    return 0;
}
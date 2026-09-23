#include <iostream>
#include <thread>

#include "../sylar/log.h"
#include "../sylar/util.h"

int main(int argc, char** argv)
{
    sylar::Logger::ptr logger(new sylar::Logger);
    logger->addAppender(sylar::LogAppender::ptr(new sylar::StdoutLogAppender));

    // sylar::LogEvent::ptr event(new sylar::LogEvent(__FILE__, __LINE__, 0, sylar::GetThreadId(), sylar::GetFiberId(), time(0)));
    // logger->log(sylar::LogLevel::DEBUG, event);

    SYLAR_LOG_DEBUG(logger) << "test macro";
    SYLAR_LOG_ERROR(logger) << "test macor error";

    SYLAR_LOG_FMT_DEBUG(logger, "str:%d, val=%d", "test macro", 2);
    SYLAR_LOG_FMT_ERROR(logger, "str:%d, val=%d", "test macro", 2);
    return 0;
}
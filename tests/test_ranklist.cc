#include "sylar/sylar.h"

sylar::Logger::ptr g_logger = SYLAR_LOG_ROOT();

int main()
{
    SYLAR_LOG_INFO(g_logger) << "Hello RankList";
}
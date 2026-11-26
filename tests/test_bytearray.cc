#include "sylar/bytearray.h"
#include "sylar/log.h"
#include "sylar/macro.h"

static sylar::Logger::ptr g_logger = SYLAR_LOG_ROOT();

void test()
{
#define XX(type, len, write_fun, read_fun, base_len) \
    std::vector<type> vec; \
    for (int i = 0; i < len; ++i) { \
        vec.push_back(rand()); \
    } \
    sylar::ByteArray::ptr ba(new sylar::ByteArray(base_len)); \
    for (auto& i : vec) { \
        ba->write_fun(i); \
    } \
    ba->setPosition(0); \
    for (size_t i = 0; i < vec.size(); ++i) { \
        int32_t v = ba->read_fun(); \
        SYLAR_ASSERT(v == vec[i]); \
    } \
    SYLAR_ASSERT(ba->getReadSize() == 0); \
    SYLAR_LOG_INFO(g_logger) << #write_fun "/" #read_fun \
        " (" #type ") len=" << len \
        << " base_len=" << base_len;

    XX(int8_t, 100, writeFint8, readFint8, 1);
#undef XX
}

int main(int argc, char** argv)
{
    test();
    return 0;
}
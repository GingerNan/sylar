#ifndef __SYLAR_UTIL_H__
#define __SYLAR_UTIL_H__

#include <unistd.h>
#include <cstdint>

namespace sylar
{

// 返回当前线程的Id
pid_t GetThreadId();

// 返回当前协程的Id
uint32_t GetFiberId();

} // namespace sylar


#endif
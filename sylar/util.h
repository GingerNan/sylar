#ifndef __SYLAR_UTIL_H__
#define __SYLAR_UTIL_H__

#include <unistd.h>
#include <cstdint>
#include <vector>
#include <string>

namespace sylar
{

// 返回当前线程的Id
pid_t GetThreadId();

// 返回当前协程的Id
uint32_t GetFiberId();

// 获取栈信息
void Backtrace(std::vector<std::string>& bt, int size, int skip = 1);

std::string BacktraceToString(int size, int skip = 2, const std::string& prefix = "");

} // namespace sylar


#endif
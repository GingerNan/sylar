#ifndef __SYLAR_UTIL_H__
#define __SYLAR_UTIL_H__

#include <unistd.h>
#include <cstdint>
#include <vector>
#include <string>

namespace sylar
{

// 获取当前线程的Id
pid_t GetThreadId();

// 获取当前协程的Id
uint32_t GetFiberId();

// 获取栈信息
void Backtrace(std::vector<std::string>& bt, int size = 64, int skip = 1);
std::string BacktraceToString(int size = 64, int skip = 2, const std::string& prefix = "");

// 毫秒
uint64_t GetCurrentMS();
// 微秒
uint64_t GetCurrentUS();    

} // namespace sylar


#endif
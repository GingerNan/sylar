#ifndef __SYLAR__HOOK_H_
#define __SYLAR__HOOK_H_

#include <unistd.h>

namespace sylar
{

bool is_hook_enable();

void set_hook_enable(bool flag);

} // namespace sylar

extern "C"
{
typedef unsigned int (*sleep_fun)(unsigned int seconds);
extern sleep_fun sleep_f;

typedef int (*usleep_fun)(useconds_t usec);
extern usleep_fun usleep_f;
}

#endif
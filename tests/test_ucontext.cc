#include "sylar/sylar.h"

#include <cstdio>
#include <unistd.h>
#include <ucontext.h>

sylar::Logger::ptr g_logger = SYLAR_LOG_ROOT();

void test_getcontext_setcontext()
{
    ucontext_t context;

    getcontext(&context);
    sleep(1);
    SYLAR_LOG_INFO(g_logger) << "hello world";
    setcontext(&context);
}

static ucontext_t uctx_main, uctx_func1, uctx_func2;

#define handler_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)
    
static void func1()
{
    SYLAR_LOG_INFO(g_logger) << "func1: started";
    SYLAR_LOG_INFO(g_logger) << "func1: swapcontext(&uctx_fun1, &uctx_fun2)";
    if(swapcontext(&uctx_func1, &uctx_func2) == -1)
    {
        handler_error("swapcontext");
    }
    SYLAR_LOG_INFO(g_logger) << "func1: returing";
}

static void func2()
{
    SYLAR_LOG_INFO(g_logger) << "func2: started";
    SYLAR_LOG_INFO(g_logger) << "func2: swapcontext(&uctx_fun2, &uctx_fun1)";
    if(swapcontext(&uctx_func2, &uctx_func1) == -1)
    {
        handler_error("swapcontext");
    }
    SYLAR_LOG_INFO(g_logger) << "func2: returing";
}

void test_makecontext_swapcontext(int argc)
{
    // 初始化协程栈空间
    char func1_stack[16384];
    char func2_stack[16384];

    // 初始化 uctx_func1，并将 uctx_func1 的后继协程指定为 uctx_main
    if(getcontext(&uctx_func1) == -1)
    {
        handler_error("getcontext");
    }
    uctx_func1.uc_stack.ss_sp = func1_stack;
    uctx_func1.uc_stack.ss_size = sizeof(func1_stack);
    uctx_func1.uc_link = &uctx_main;
    makecontext(&uctx_func1, func1, 0);

    // 初始化 utx_fun2，并将 uctx_func2 的后继协程指定为 uctx_func1 或 结束
    if(getcontext(&uctx_func2) == -1)
    {
        handler_error("getcontext");
    }
    uctx_func2.uc_stack.ss_sp = func2_stack;
    uctx_func2.uc_stack.ss_size = sizeof(func2_stack);\
    uctx_func2.uc_link = (argc > 1) ? NULL : &uctx_func1;
    makecontext(&uctx_func2, func2, 0);

    SYLAR_LOG_INFO(g_logger) << "main: swapcontext(&uctx_main, &uctx_func2)";
    if(swapcontext(&uctx_main, &uctx_func2) == -1)
    {
        handler_error("swapcontext");
    }

    SYLAR_LOG_INFO(g_logger) << "main: exiting";
}

int main(int argc, char** argv)
{
    SYLAR_LOG_INFO(g_logger) << "argc=" << argc; 
    // test_getcontext_setcontext();
    test_makecontext_swapcontext(argc);
}
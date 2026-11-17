#include "sylar/sylar.h"

#include <vector>
#include <ucontext.h>

sylar::Logger::ptr g_logger = SYLAR_LOG_ROOT();

#define STACK_SIZE 1024 * 10

ucontext_t main_context, sub_context;
int count = 0;

void func1()
{
    while (count++ < 10)
    {
        SYLAR_LOG_INFO(g_logger) << "func1 start";
        // 保存当前cpu的状态到 sub_context, 并切换到 main_context 执行
        swapcontext(&sub_context, &main_context);
        SYLAR_LOG_INFO(g_logger) << "func1 end";
    }
}

void func2()
{
    while (count++ < 10)
    {
        SYLAR_LOG_INFO(g_logger) << "func2 start";
        // 保存当前cpu的状态到 main_context, 切换到 sub_context 执行
        swapcontext(&main_context, &sub_context);       
        SYLAR_LOG_INFO(g_logger) << "func2 end";
    }
}

void test_ucontext()
{
    char* stack = (char*)malloc(STACK_SIZE);
    
    // 初始化 sub_context
    getcontext(&sub_context);
    // 设置 sub_context 的堆栈大小
    sub_context.uc_stack.ss_sp = stack;
    sub_context.uc_stack.ss_size = STACK_SIZE;
    // 设置 sub_context 完成后下一个执行的协程
    sub_context.uc_link = &main_context;
    
    // 给 func1 设置一个新的入口函数，当激活 sub_context 这个协程时，调用fun1函数
    makecontext(&sub_context, func1, 0);

    func2();

    free(stack);
}

void run_in_fiber()
{
    SYLAR_LOG_INFO(g_logger) << "run_in_fiber begin";
    sylar::Fiber::YeildToHold();
    SYLAR_LOG_INFO(g_logger) << "run_in_fiber end";
    sylar::Fiber::YeildToHold();
}

void test_fiber()
{
    SYLAR_LOG_INFO(g_logger) << "main begin -1";
    {
        sylar::Fiber::GetThis();
        SYLAR_LOG_INFO(g_logger) << "main begin";
        sylar::Fiber::ptr fiber(new sylar::Fiber(run_in_fiber));
        fiber->swapIn();
        SYLAR_LOG_INFO(g_logger) << "main after swapIn";
        fiber->swapIn();
        SYLAR_LOG_INFO(g_logger) << "main after end";
        fiber->swapIn();
    }
    SYLAR_LOG_INFO(g_logger) << "main after end2";
}

int main(int argc, char** argv)
{
    // sylar::Thread::SetName("main");

    // std::vector<sylar::Thread::ptr> thrs;
    // for (int i = 0; i < 3; ++i)
    // {
    //     thrs.push_back(sylar::Thread::ptr(
    //         new sylar::Thread(test_fiber, "name_" + std::to_string(i))));
    // }

    // for (auto i : thrs)
    // {
    //     i->join();
    // }

    test_ucontext();

    return 0;
}
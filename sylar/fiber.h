#ifndef __SYLAR_FIBER_H__
#define __SYLAR_FIBER_H__

#include <memory>
#include <ucontext.h>
#include <functional>

#include "thread.h"

namespace sylar
{

class Scheduler;

/**
 * @brief 协程类
 */
class Fiber : public std::enable_shared_from_this<Fiber>
{
    friend class Scheduler;
public:
    using ptr = std::shared_ptr<Fiber>;

    enum State {
        INIT,   // 初始状态
        HOLD,   // 暂停状态
        EXEC,   // 执行中状态
        TERM,   // 结束状态
        READY,  // 可执行状态
        EXCEPT  // 异常状态
    };
private:
    Fiber();

public:
    Fiber(std::function<void()> cb, size_t stacksize = 0);
    ~Fiber();

    // 重置协程函数，并重置状态
    // INIT, TERM
    void reset(std::function<void()> cb);
    // 切换到当前协程执行
    void swapIn();
    // 切换到后台执行
    void swapOut();

    void call();

    uint64_t getId() const { return m_id; }
    State getState() const { return m_state; }

public:
    // 设置当前协程
    static void SetThis(Fiber* f);
    // 返回当前协程
    static Fiber::ptr GetThis();
    // 协程切换到后台，并且设置为Ready可执行状态
    static void YeildToReady();
    // 协程切换到后台，并且设置为Hold暂停状态
    static void YeildToHold();
    // 总协程数
    static uint64_t TotalFibers();

    static void MainFunc();
    static uint64_t GetFiberId();
private:
    uint64_t m_id = 0;
    uint32_t m_stacksize = 0;
    State m_state = INIT;

    ucontext_t m_ctx;
    void* m_stack = nullptr;


    std::function<void()> m_cb;
};

} // namespace sylar

#endif
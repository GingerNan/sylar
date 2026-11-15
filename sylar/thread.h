#ifndef __SYLAR_THREAD_H__
#define __SYLAR_THREAD_H__

#include <memory>
#include <string>
#include <functional>
#include <pthread.h>
#include <cstdint>

#include "mutex.h"

namespace sylar
{

class Thread
{
public:
    using ptr = std::shared_ptr<Thread>;
    Thread(std::function<void()> cb, const std::string& name);
    ~Thread();

    pid_t getId() const { return m_id; }
    const std::string& getName() const { return m_name; }

    // 等待线程执行完成
    void join();

    // 获取当前线程指针
    static Thread* GetThis();
    // 获取当前的线程名称
    static const std::string& GetName();
    // 设置当前线程名称
    static void SetName(const std::string& name);

private:
    Thread(const Thread&) = delete;
    Thread(Thread&&) = delete;
    Thread& operator=(const Thread&) = delete;
    Thread& operator=(Thread&&) = delete;

    // 线程执行函数
    static void* run(void* arg);
private:
    pid_t m_id = -1;                // 线程id
    pthread_t m_thread = 0;         // 线程结构
    std::function<void()> m_cb;     // 线程执行函数
    std::string m_name;             // 线程名称

    Semaphore m_semaphore;          // 信号量
};

} // namespace sylar


#endif
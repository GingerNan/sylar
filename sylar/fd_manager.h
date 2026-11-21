#ifndef __FD_MANAGER_H__
#define __FD_MANAGER_H__

#include <memory>
#include <vector>

#include "iomanager.h"
#include "thread.h"
#include "singleton.h"

namespace sylar
{

/**
 * @brief 文件句柄上下文类
 */
class FdCtx : public std::enable_shared_from_this<FdCtx>
{
public:
    using ptr = std::shared_ptr<FdCtx>;

    FdCtx(int fd);
    ~FdCtx();

    bool init();
    bool isInit() const { return m_isInit; }
    bool isSocket() const { return m_isSocket; }
    bool isClosed() const { return m_isClosed; }

    void setUserNonblock(bool v) { m_userNonblock = v; }
    bool getUserNonblock() const { return m_userNonblock; }

    void setSysNonblock(bool v) { m_sysNonblock = v; }
    bool getSysNonblock() const { return m_sysNonblock; }

    void setTimeout(int type, uint64_t v);
    uint64_t getTimeout(int type) const;

private:
    bool m_isInit: 1;
    bool m_isSocket: 1;
    bool m_sysNonblock: 1;
    bool m_userNonblock: 1;
    bool m_isClosed: 1;
    int m_fd;

    uint64_t m_recvTimeout;
    uint64_t m_sendTimeout;
    IOManager* m_iomanager;
};

/**
 * @brief 文件句柄管理类
 */
class FdManager
{
public:
    using RWMutexType = RWMutex;
    FdManager();

    FdCtx::ptr get(int fd, bool auto_create = false);
    void del(int fd);
private:
    RWMutexType m_mutex;
    std::vector<FdCtx::ptr> m_datas;
};

using FdMgr = Singleton<FdManager>;

} // namespace sylar




#endif
#include "sylar/sylar.h"
#include "sylar/iomanager.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>

sylar::Logger::ptr g_logger = SYLAR_LOG_ROOT();

void test_fiber()
{
    SYLAR_LOG_INFO(g_logger) << "test_fiber";
}

void test1()
{
    /** BUG
     *   在 use_caller 传入 false 的时候，会导致 Scheduler::t_scheduler 一直为空指针，
     * 在 IOManager::triggerEvent() 中调用 ctx.scheduler 也是 nullptr 这里会段错误。
     */ 
    sylar::IOManager iom(2, false);
    iom.scheduler(&test_fiber);

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    fcntl(sock, F_SETFL, O_NONBLOCK);

    sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9999);
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr.s_addr);
    
    connect(sock, (const sockaddr*)&addr, sizeof(addr));

    iom.addEvent(sock, sylar::IOManager::READ, [](){
        SYLAR_LOG_INFO(g_logger) << "read callback";
    });

    iom.addEvent(sock, sylar::IOManager::WRITE, [sock](){
        SYLAR_LOG_INFO(g_logger) << "write callback";
        sylar::IOManager::GetThis()->cancelEvent(sock, sylar::IOManager::READ);
        close(sock);
    });

}

int main(int argc, char** argv)
{
    test1();
    return 0;
}
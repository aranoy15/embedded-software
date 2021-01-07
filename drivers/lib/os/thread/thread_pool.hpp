#ifndef DRIVERS_LIB_OS_THREAD_THREAD_POOL
#define DRIVERS_LIB_OS_THREAD_THREAD_POOL

#include <drivers/lib/pool_base.hpp>
#include <drivers/lib/singleton.hpp>
#include <drivers/lib/os/thread/thread.hpp>

namespace lib::os::thread
{
class ThreadPool : public PoolBase<Thread>, public Singleton<ThreadPool>
{
public:
    ThreadPool(const ThreadPool&) = delete;
    ThreadPool operator=(const ThreadPool&) = delete;

    ThreadPool() : PoolBase(), Singleton() {}
    virtual ~ThreadPool() {}

    void start()
    {
        for (auto& item : _data) {
            item->start();
        }
    }
};
}

#endif /* DRIVERS_LIB_OS_THREAD_THREAD_POOL */

#include <drivers/lib/os/thread/thread_pool.hpp>
#include <drivers/lib/uart/uart.hpp>
#include <drivers/lib/uart/gprs/sim900.hpp>
#include <bsp.hpp>
#include <drivers/lib/utils.hpp>

namespace logic::thread
{
using thread_t = lib::os::thread::Thread;
using priority_t = bsp::os::thread::priority_t;

class GprsThread : public thread_t
{
public:
	GprsThread() : thread_t(priority_t::High, 4 * 1024) {}

    std::string_view name() const override { return "gprs"; }
    void func() override;
};
}

namespace 
{
__attribute__((constructor))
void reg()
{
    lib::os::thread::ThreadPool::instance()->reg<logic::thread::GprsThread>();
}
}

void logic::thread::GprsThread::func()
{
    using namespace lib::uart;
    using namespace lib::uart::gprs;

    using time_t = lib::time::Time;

    Uart<bsp::usart::sim900_p>::init();

    Sim900::init(); 

    loop {
        time_t::sleep(time_t::msecs(100));
    }
}

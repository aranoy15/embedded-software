#include <lib/os/thread/thread_pool.hpp>
#include <lib/uart/uart.hpp>
#include <bsp.hpp>
#include <lib/utils.hpp>
#include <lib/time/time.hpp>
#include <lib/uart/log/log.hpp>
#include <lib/stream/actions.hpp>
#include <lib/uart/gsm/gsm.hpp>

namespace logic::thread
{
using thread_t = lib::os::thread::Thread;
using priority_t = bsp::os::thread::priority_t;

class GsmThread : public thread_t
{
public:
	GsmThread() : thread_t(priority_t::High, 4 * 1024) {}

    std::string_view name() const override { return "gprs"; }
    void func() override;
};
}

namespace 
{
__attribute__((constructor))
void reg()
{
    lib::os::thread::ThreadPool::instance()->reg<logic::thread::GsmThread>();
}
}

void logic::thread::GsmThread::func()
{
    using namespace lib::uart;
    using namespace lib::uart::log;
    using namespace lib::stream;

    using time_t = lib::time::Time;

    gsm::Gsm::uart_t::init();

    std::string data;

    gsm::Gsm::init();

    loop {

    }
}

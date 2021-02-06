#include <bsp.hpp>

#include <logic/test/logic.hpp>
#include <lib/time/timer.hpp>
#include <lib/uart/uart.hpp>
#include <lib/uart/log/log.hpp>
#include <lib/os/thread/thread.hpp>
#include <lib/os/thread/thread_pool.hpp>
#include <string>
#include <vector>
#include <memory>
#include <lib/utils.hpp>

void main_thread(void*)
{
    using namespace lib::os::thread;
    using namespace lib::uart::log;
    using namespace lib::stream;

    using time_t = lib::time::Time;

    Log() << "Start application" << Endl();

    ThreadPool::instance()->start();

    loop {
        bsp::wdt::reset();
        time_t::sleep(time_t(100));
    }
}

void applogic::start()
{
    using namespace lib::uart::log;
    using namespace lib::os::thread;
    using namespace bsp::os;

    using time_t = lib::time::Time;
    using timer_t = lib::time::Timer;
    using priority_t = bsp::os::thread::priority_t;

    bsp::gpio::init();
    LogHandler::uart_t::init(); 
    bsp::os::init();
    bsp::wdt::init();

    Thread::start(main_thread, "main", 256, priority_t::Realtime);
    bsp::os::start();
}

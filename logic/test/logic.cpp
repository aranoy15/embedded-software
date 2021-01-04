#include <logic/test/logic.hpp>
#include <drivers/bsp/F401xE_TEST/bsp.hpp>
#include <drivers/lib/time/timer.hpp>
#include <string>
#include <vector>

#define loop while(true)

void test_thread(void* args)
{
    using time_t = lib::time::Time;

    loop {
        bsp::gpio::status::toggle();
        time_t::sleep(time_t(250));
    }
}

void applogic::start()
{
    using time_t = lib::time::Time;
    using timer_t = lib::time::Timer;

    bsp::gpio::init();
    bsp::os::init();

    auto handle_default = bsp::os::reg(test_thread, nullptr, "df", 256, bsp::os::Priority::High);
    bsp::os::start();
}

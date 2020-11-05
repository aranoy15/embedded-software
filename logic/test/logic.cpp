#include <logic/test/logic.hpp>
#include <bsp.hpp>
#include <drivers/lib/time/timer.hpp>
#include <drivers/lib/uart/uart.hpp>
#include <drivers/lib/uart/log/log.hpp>
#include <string>

#define loop while(true)

void applogic::start()
{
    using time_t = lib::time::Time;
    using timer_t = lib::time::Timer;

    using namespace lib::uart::log;

    LogHandler::uart_t::init();

    Log() << "Start application";

    bsp::gpio::init();

    bsp::iwdg::init();

    time_t timeout = time_t(250);

    timer_t blink_timer(timeout);

    loop {
        if (blink_timer.elapsed()) {
            static bool state = false;

            if (state) bsp::gpio::status::on();
            else bsp::gpio::status::off();

            state = not state;

            blink_timer.start();
        }
    }
}

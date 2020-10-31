#include <logic/test/logic.hpp>
#include <bsp.hpp>

#define loop while(true)

void applogic::start()
{
    bsp::gpio::init();
    uint32_t timeout = 200;

    loop {
        bsp::gpio::status_on();
        bsp::tick::delay(timeout);
        bsp::gpio::status_off();
        bsp::tick::delay(timeout);
    }
}

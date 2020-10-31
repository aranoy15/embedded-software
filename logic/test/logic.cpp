#include <logic/test/logic.hpp>
#include <bsp.hpp>

#define loop for(;;)

void applogic::start()
{
    bsp::gpio::init();
    uint32_t timeout = 100;

    loop {
        bsp::gpio::status_on();
        bsp::tick::delay(timeout);
        bsp::gpio::status_off();
        bsp::tick::delay(timeout);
    }
}

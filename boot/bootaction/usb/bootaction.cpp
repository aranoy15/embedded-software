#include <bsp.hpp>
#include <gpio.h>

void boot_action()
{
    bsp::usb::init();

    bsp::tick::delay(1000);

    //uint8_t data[] = "Hello world!!!\r\n";

    for (;;) {
        //bsp::usb::send(data, sizeof(data));
        bsp::tick::delay(1000);
    }
}

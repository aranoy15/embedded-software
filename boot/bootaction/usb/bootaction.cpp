#include <bsp.hpp>

void boot_action()
{
    bsp::usb::init();

    //uint8_t data[] = "Hello world!!!\r\n";

    for (;;) {
        //bsp::usb::send(data, sizeof(data));
        //bsp::tick::delay(1000);
    }
}

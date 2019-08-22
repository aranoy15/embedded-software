#include <logic.h>
#include <gpio.h>
#include <time.h>
#include <vector>
#include <uart.h>
#include <utils.h>
#include <sdcard.h>

using namespace gpio;

void applogic::startLogic()
{
    using led = GPIO<PinDef<CSP_GPIO_PORT_NBR_C, GPIO_PIN_13>, mOutputPP>;
    Uart<uart::UartPort::usart1>& log = *Uart<uart::UartPort::usart1>::instance();

    log.init(128, 115200); 
    led::setup();

	for (;;) {
        led::on();
        Time::sleep(Time(500));
        led::off();
        Time::sleep(Time(500));
    }
}
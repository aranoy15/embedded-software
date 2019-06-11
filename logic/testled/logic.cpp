#include <logic.h>
#include <gpio.h>
#include <time.h>

using namespace gpio;

void applogic::startLogic()
{
    using led = GPIO<PinDef<CSP_GPIO_PORT_NBR_C, GPIO_PIN_13>, mOutputPP>;
    led::setup();

	for (;;) {
        led::on();
        Time::sleep(Time(500));
        led::off();
        Time::sleep(Time(500));
    }
}
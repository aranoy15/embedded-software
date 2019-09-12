#include <logic.h>
#include <time.h>
#include <bsp.h>
#include <i2c.h>
#include <uart.h>

const uint16_t lcdAddress = 0x27;

void applogic::startLogic()
{
  using led = GPIO<PinDef<CSP_GPIO_PORT_NBR_C, GPIO_PIN_13>, bsp::mOutputPP>;

  Uart<bsp::log::logPort>& log = *Uart<bsp::log::logPort>::instance();
  log.init(128, 115200);

  led::setup();

	for (;;) {
    auto test = log.readln(50);
    if (not test.empty())
      log.send(utils::stringFormat("Receive message: %s", test.c_str()));

    led::off();
    Time::sleep(Time(500));
    led::on();
    Time::sleep(Time(500));
	}
}
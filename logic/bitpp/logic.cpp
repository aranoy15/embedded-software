#include <bsp.h>
#include <flash.h>
#include <logic.h>
#include <time.h>
#include <uart.h>
#include <utils.h>

#include <fatfsclass.h>

const uint16_t lcdAddress = 0x27;

void applogic::startLogic()
{
	using led = GPIO<PinDef<CSP_GPIO_PORT_NBR_C, GPIO_PIN_13>, bsp::mOutputPP>;
	using LogUart = Uart<bsp::log::logPort>;
	LogUart& log = *LogUart::instance();

	log.init(128, 115200);

	led::setup();

	log.send(std::string("Start main\r"));

    uint32_t count = 0;
	bool state = false;

	for (;;) {
		if (state) led::off();
		else led::on();

		state = not state;

		auto readMessage = log.readln(1000);
		if (not readMessage.empty())
			log.send(utils::stringFormat("Result read: %s\r", readMessage.c_str()));

		log.send(std::string("Test message\r"));
	}
}
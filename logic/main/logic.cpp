#include <bsp.h>
#include <i2c.h>
#include <logic.h>
#include <thread.h>
#include <time.h>
#include <uart.h>
#include <bme280.h>

const uint16_t lcdAddress = 0x27;

class BlinkThread : public Thread
{
public:
	BlinkThread() : Thread(0, 512) {}

	const char* threadName() override { return "blink"; }

	void threadFunc() override
	{
		using led = GPIO<PinDef<CSP_GPIO_PORT_NBR_C, GPIO_PIN_13>, bsp::mOutputPP>;
		led::setup();

        for (;;) {
            led::on();
            Time::sleep(Time(500));
            led::off();
            Time::sleep(Time(500));
        }
	}
};

class TempThread : public Thread
{
private:
    static constexpr uint16_t stackSize = 2 * 1024;

public:
	TempThread() : Thread(2, stackSize) {}

	const char* threadName() override { return "temp"; }

	void threadFunc() override
	{
        I2C<bsp::i2cP1>& i2c = *I2C<bsp::i2cP1>::instance();
        Uart<bsp::log::logPort>& log = *Uart<bsp::log::logPort>::instance();
        i2c.init();

        auto addresses = i2c.searchAddresses();

        Bme280 bme(0x76);

        bool resInit = bme.init();

        log.send(utils::stringFormat("Bme result init: %s", resInit ? "yes" : "no"));

        for (;;) {

            Time::sleep(Time::secs(1));
        }
	}
};

void applogic::startLogic()
{
	Uart<bsp::log::logPort>& log = *Uart<bsp::log::logPort>::instance();
	log.init(128, 115200);

    BlinkThread bt;
    TempThread tt;

    bt.start();
    tt.start();

	for (;;) {
		auto readMessage = log.readln(1000);
		if (not readMessage.empty())
			log.send(
			    utils::stringFormat("Result read: %s\r", readMessage.c_str()));

		log.send(std::string("Test message\r"));
        Time::sleep(Time(10));
	}
}
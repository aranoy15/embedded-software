#include <logic.h>
#include <bsp.h>
#include <time.h>
#include <vector>
#include <uart.h>
#include <utils.h>
#include <i2c.h>
#include <mhz19.h>

void applogic::startLogic()
{
    Uart<bsp::uartP1>& log = *Uart<bsp::uartP1>::instance();
    I2C<bsp::i2cP1>& i2c = *I2C<bsp::i2cP1>::instance();

    i2c.init();
    log.init(128, 115200); 
    bsp::led::signal::setup();

    log.send("Start application");

    //std::string result = "Addresses:";
	//for (auto address : i2c.searchAddresses()) {
	//	result += utils::stringFormat(" %02X", address);
	//}
	//log.send(result);

    Mhz19 mhz;
    mhz.init();

    mhz.setAutoCalibration(false);

	for (;;) {

        bsp::led::signal::on();
        Time::sleep(Time(500));
        bsp::led::signal::off();
        Time::sleep(Time(500));

        uint16_t co2 = mhz.getCO2();

        log.send(utils::stringFormat("CO2: %u", co2));
    }
}
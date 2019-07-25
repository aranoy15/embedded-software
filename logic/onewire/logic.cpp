#include <logic.h>
#include <gpio.h>
#include <time.h>
#include <uart.h>
#include <utils.h>
#include <onewire.h>
#include <i2c.h>
#include <mhz19.h>
#include <lcd.h>
#include <watchdog.h>

using namespace gpio;

void applogic::startLogic()
{
    Watchdog::init(3);
    Watchdog::start();

    Uart<uart::UartPort::usart1>& log = *Uart<uart::UartPort::usart1>::instance();
    log.init(128, 115200); 

    log.send("Start application");

    using led = GPIO<PinDef<CSP_GPIO_PORT_NBR_C, GPIO_PIN_13>, mOutputPP>;
    led::setup();

    using MainI2cType = I2C<i2c::i2cPort1>;
    MainI2cType& mainI2C = *MainI2cType::instance();
    mainI2C.init();

    Lcd lcd(0x27, 20, 4, 1);
    lcd.init();

    Mhz19 mhz19;
    mhz19.init();

    mhz19.setAutoCalibration(false);

    std::vector<uint8_t> addresses;

	for (;;) {
        Watchdog::reload();

        addresses = mainI2C.searchAddresses();

        log.send("Addresses count: " + std::to_string(addresses.size()));

        led::on();
        Time::sleep(Time(500));
        led::off();
        Time::sleep(Time(500));

        uint16_t co2 = mhz19.getCO2();

        lcd.setCursor(16, 0);
        lcd.sendString(std::to_string(co2));
    }
}
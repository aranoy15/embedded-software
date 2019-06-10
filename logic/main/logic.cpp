#include <logic.h>
#include <gpio.h>
#include <time.h>
#include <uart.h>
#include <i2c.h>
#include <lcd.h>
#include <math.h>
#include <utils.h>
#include <bme280.h>

using namespace gpio;

void applogic::startLogic()
{
    using led = GPIO<PinDef<CSP_GPIO_PORT_NBR_C, GPIO_PIN_13>, mOutputPP>;
    led::setup();

    using LogUart = Uart<uart::UartPort::usart1>;
    LogUart& log = *LogUart::instance();

    using MainI2cType = I2C<i2c::i2cPort1>;
    MainI2cType& mainI2C = *MainI2cType::instance();

    log.init(128, 115200);
    mainI2C.init();
    log.send("Start application");

    std::vector<uint8_t> addresses = mainI2C.searchAddresses();

    if (addresses.empty())
        log.send("No i2c devices");
    else {
        for (const uint8_t address : addresses) {
            char hex[10];
            sprintf(hex, "0x%02X", address);
            log.send(std::string("Find device number: ") + std::string(hex));
        }
    }

    Bme280 bme(0x76);
    bool resultInit = bme.init();

    for (;;) {
        if (resultInit) {
            log.send(std::string("Temperature = ") + utils::ftostring(bme.readTemperature()) + " *C");
            log.send(std::string("Pressure = ") + utils::ftostring(bme.readPressure() / 100.0f) + " hPa");
            log.send(std::string("Humidity = ") + utils::ftostring(bme.readHumidity()) + " %");
            log.send(" ");
        }

        led::on();
        Time::sleep(Time(500));
        led::off();
        Time::sleep(Time(500));
    }
}
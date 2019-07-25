#include <logic.h>
#include <gpio.h>
#include <time.h>
#include <vector>
#include <uart.h>
#include <i2c.h>
#include <utils.h>
#include <mhz19.h>
#include <lcd.h>

using namespace gpio;

void applogic::startLogic()
{
    using led = GPIO<PinDef<CSP_GPIO_PORT_NBR_C, GPIO_PIN_13>, mOutputPP>;
    Uart<uart::UartPort::usart1>& log = *Uart<uart::UartPort::usart1>::instance();
    I2C<i2c::i2cPort1>& i2c = *I2C<i2c::i2cPort1>::instance();

    using red = GPIO<PinDef<CSP_GPIO_PORT_NBR_A, GPIO_PIN_5>, mOutputPP>;
    using blue = GPIO<PinDef<CSP_GPIO_PORT_NBR_A, GPIO_PIN_6>, mOutputPP>;
    using green = GPIO<PinDef<CSP_GPIO_PORT_NBR_A, GPIO_PIN_7>, mOutputPP>;

    red::setup();
    blue::setup();
    green::setup();

    log.init(128, 115200); 
    i2c.init();
    led::setup();


    std::vector<uint8_t> addresses = i2c.searchAddresses();

    if (not addresses.empty()) {
        log.dump(addresses.data(), addresses.size(), "Addresses: ");
    }

    Timer blinkTimer(500);
    Timer infoTimer(1000);

    blinkTimer.start();
    infoTimer.start();

    bool blinkState = false;
    
    Lcd lcd(0x27, 20, 4, 1);
    lcd.init();

    lcd.sendString("Hello world!");
    Mhz19 mhz;
    mhz.init();

    mhz.setAutoCalibration(false);
    mhz.getCO2();

	for (;;) {

        if (infoTimer.elapsed()) {
            int freeStack = utils::freeStack();
            log.send("Free stack = " + std::to_string(freeStack));
            log.send(" ");
            
            uint16_t co2 = mhz.getCO2();
            lcd.setCursor(0, 1);
            lcd.sendString("CO2: " + std::to_string(co2));
            infoTimer.start();
        }

        if (blinkTimer.elapsed()) {
            blinkState = not blinkState;
            if (blinkState) led::on();
            else led::off();
            blinkTimer.start();
        }
    }
}
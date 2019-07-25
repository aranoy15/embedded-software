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
#include <lcdlogic.h>
#include <clocklogic.h>
#include <sensorlogic.h>

using namespace gpio;

void applogic::startLogic()
{
    Watchdog::init(3);
    Watchdog::start();

    Uart<uart::UartPort::usart1>& log = *Uart<uart::UartPort::usart1>::instance();
    log.init(128, 115200); 

    I2C<i2c::i2cPort1>::instance()->init();;

    SensorLogic::instance()->tempInit();
    SensorLogic::instance()->co2Init();

    //std::unique_ptr<Bme280> bme(new Bme280(0x76));
    //bme->init();

    log.send("Start application");

    //using led = GPIO<PinDef<CSP_GPIO_PORT_NBR_C, GPIO_PIN_13>, mOutputPP>;
    //led::setup();


    /* 
    Lcd lcd(0x27, 20, 4, 1);
    lcd.init();

    Mhz19 mhz19;
    mhz19.init();

    mhz19.setAutoCalibration(false);
    */

	for (;;) {
        Watchdog::reload();

        SensorLogic::instance()->processSensor();
        ClockLogic::instance()->processClock();
        LcdLogic::instance()->processLcd();
        

        //uint16_t co2 = mhz19.getCO2();
        //lcd.setCursor(16, 0);
        //lcd.sendString(std::to_string(co2));
    }
}
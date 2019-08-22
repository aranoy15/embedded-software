#include <logic.h>
#include <gpio.h>
#include <time.h>
#include <uart.h>
#include <i2c.h>
#include <watchdog.h>
#include <lcdlogic.h>
#include <clocklogic.h>
#include <sensorlogic.h>
#include <datastorage.h>
#include <ledlogic.h>
#include <logicstate.h>

using namespace gpio;

void applogic::startLogic()
{
    Watchdog::init(3);
    Watchdog::start();

    Uart<uart::UartPort::usart1>& log = *Uart<uart::UartPort::usart1>::instance();
    log.init(128, 115200); 

    I2C<i2c::i2cPort1>::instance()->init();;

    LogicState::instance()->init();

    SensorLogic::instance()->tempInit();
    SensorLogic::instance()->co2Init();

    log.send("Start application");

	for (;;) {
        Watchdog::reload();

        LogicState::instance()->process();
        SensorLogic::instance()->processSensor();
        ClockLogic::instance()->processClock();
        LcdLogic::instance()->processLcd();
        LedLogic::instance()->processLed();
        DataStorage::instance()->processDataStorage();
    }
}
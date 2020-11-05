#include <logic/meteostation/logic.hpp>
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

void applogic::startLogic()
{
    Watchdog::init(3);
    Watchdog::start();

    Uart<bsp::UartPort::uartP1>& log = *Uart<bsp::UartPort::uartP1>::instance();
    log.init(128, 115200); 

    I2C<bsp::i2cP1>::instance()->init();;

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

        std::string line = log.readln(10);

        if (not line.empty()) {
            if (line.rfind("AT+SETDATETIME ", 0) == 0) {
                line.erase(0, 15);
                DateTime dt(line.c_str());
                ClockLogic::instance()->setDateTime(dt);
                log.send("AT+SETDATETIME:OK");
            }
        }
    }
}
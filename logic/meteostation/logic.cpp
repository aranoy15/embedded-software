#include <bsp.hpp>
#include <logic/meteostation/logic.hpp>
#include <drivers/lib/uart/log/log.hpp>
#include <drivers/lib/time/timer.hpp>
#include <drivers/lib/task/scheduler.hpp>
#include <logic/meteostation/dologic/sensorlogic.hpp>
#include <logic/meteostation/dologic/lcdlogic.hpp>

#define loop while(true)

void applogic::start()
{
    using namespace lib::time;
    using namespace lib::uart::log;
    using namespace lib::task;

    LogHandler::uart_t::init();
    bsp::gpio::init();
    //bsp::iwdg::init();

    Scheduler::reg<applogic::SensorLogic>();
    Scheduler::reg<applogic::LcdLogic>();

    Log() << "Start application" << lib::stream::Endl();

    Scheduler::start();

    /*
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
    */
}
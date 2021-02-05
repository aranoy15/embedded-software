#include <bsp.hpp>
#include <logic/meteostation/logic.hpp>
#include <lib/uart/log/log.hpp>
#include <lib/time/timer.hpp>
#include <lib/task/scheduler.hpp>
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
}
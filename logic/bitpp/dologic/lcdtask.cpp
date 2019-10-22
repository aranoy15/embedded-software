#include <lcdtask.h>
#include <fatfsclass.h>
//#include <datetimemanager.h>
#include <flash.h>
#include <datastorage.h>
#include <watchdog.h>
#include <timer.h>

namespace
{
__attribute__((constructor)) void reg()
{
	TaskScheduler::instance()->reg(new LcdTask());
}
}  // namespace

LcdTask::LcdTask()
    : TaskBase(Time::second(), TaskPriority::Low),
      lcd1(lcd1Address, 20, 4, 1),
      lcd2(lcd2Address, 20, 4, 1)
{
}

LcdTask::~LcdTask() {}

void LcdTask::setup()
{
    lcd1.init();
    lcd2.init();
}

void LcdTask::func()
{
	static Timer timer(120000);
	Watchdog::reload();

	if (timer.elapsed()) {
		lcd1.init();
		lcd2.init();
		timer.start();
	}

    lcd1.setCursor(0, 0);
	lcd1.sendString(utils::stringFormat(
	    "Davl isp: %-10.1f", DataStorage::instance()->boilingPressure()));

	lcd1.setCursor(0, 1);
	lcd1.sendString(utils::stringFormat(
	    "Davl kon: %-10.1f", DataStorage::instance()->condansatingPressure()));

	lcd1.setCursor(0, 2);
	lcd1.sendString(utils::stringFormat(
	    "Temp isp: %-10.1f", DataStorage::instance()->boilingTemp()));

	lcd1.setCursor(0, 3);
	lcd1.sendString(utils::stringFormat(
	    "Temp kon: %-10.1f", DataStorage::instance()->condansationTemp()));

	lcd2.setCursor(0, 0);
	lcd2.sendString(utils::stringFormat(
	    "Tkon: %-10.1f", DataStorage::instance()->condensatorTemp()));

	lcd2.setCursor(0, 1);
	lcd2.sendString(utils::stringFormat(
	    "Tisp: %-10.1f", DataStorage::instance()->evaporatorTemp()));

	lcd2.setCursor(0, 2);
	lcd2.sendString(utils::stringFormat(
	    "Temp pg: %-10.1f", DataStorage::instance()->overHeatingTemp()));

	lcd2.setCursor(0, 3);
	lcd2.sendString(utils::stringFormat(
	    "Temp po: %-7.1f", DataStorage::instance()->overCoolingTemp()));
}
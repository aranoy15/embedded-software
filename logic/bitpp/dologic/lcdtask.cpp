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

	/*
    lcd1.setCursor(0, 0);
	lcd1.sendString(utils::stringFormat(
	    "\340a\263\273 \270c\276: %-10.1f", DataStorage::instance()->boilingPressure())); //Давл исп

	lcd1.setCursor(0, 1);
	lcd1.sendString(utils::stringFormat(
	    "\340a\263\273 \272o\275: %-10.1f", DataStorage::instance()->condansatingPressure())); // Давл кон
	*/

	lcd1.setCursor(0, 0);
	lcd1.sendString(utils::stringFormat(
	    "\340 \270c\276: %-4.1f", DataStorage::instance()->boilingPressure())); //Д исп

	lcd1.setCursor(12, 0);
	lcd1.sendString(
	    utils::stringFormat("%8.1f", DataStorage::instance()->minBoiling()));

	lcd1.setCursor(0, 1);
	lcd1.sendString(utils::stringFormat(
	    "\340 \272o\275: %-4.1f", DataStorage::instance()->condansatingPressure())); //Д кон

	lcd1.setCursor(12, 1);
	lcd1.sendString(
	    utils::stringFormat("%8.1f", DataStorage::instance()->maxCondansating()));

	lcd1.setCursor(0, 2);
	lcd1.sendString(utils::stringFormat(
	    "Te\274\276 \270c\276: %-10.1f", DataStorage::instance()->boilingTemp())); // Темп исп

	lcd1.setCursor(0, 3);
	lcd1.sendString(utils::stringFormat(
	    "Te\274\276 \272o\275: %-10.1f", DataStorage::instance()->condansationTemp())); // Темп кон

	lcd2.setCursor(0, 0);
	lcd2.sendString(utils::stringFormat(
	    "\340a\277 \277e\274\276 \270c\276: %-6.1f", DataStorage::instance()->evaporatorTemp())); //Дат темп исп

	lcd2.setCursor(0, 1);
	lcd2.sendString(utils::stringFormat(
	    "\340a\277 \277e\274\276 \272o\275: %-6.1f", DataStorage::instance()->condensatorTemp())); //Дат темп кон

	lcd2.setCursor(0, 2);
	lcd2.sendString(utils::stringFormat(
	    "Te\274\276 \276\264: %-10.1f", DataStorage::instance()->overHeatingTemp())); //Темп пг

	lcd2.setCursor(0, 3);
	lcd2.sendString(utils::stringFormat(
	    "Te\274\276 \276o: %-7.1f", DataStorage::instance()->overCoolingTemp())); //Темп по
}
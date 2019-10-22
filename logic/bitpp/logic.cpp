#include <bsp.h>
#include <flash.h>
#include <logic.h>
#include <time.h>
#include <utils.h>
#include <i2c.h>
#include <lcd.h>
#include <usb.h>
#include <spi.h>
#include <flash.h>
#include <taskscheduler.h>
//#include <datetimemanager.h>
#include <watchdog.h>

void applogic::startLogic()
{
	Watchdog::init(7);
	Watchdog::start();

	UsbDevice::instance()->init();

	I2C<bsp::i2cP1>::instance()->init();
	Spi<bsp::spiP1>::instance()->init();	
	Spi<bsp::spiP2>::instance()->init();

	//DateTimeManager::instance()->init();

	TaskScheduler::instance()->start();
}
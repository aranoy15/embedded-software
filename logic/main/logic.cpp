#include <bsp.h>
#include <i2c.h>
#include <lcd.h>
#include <logic.h>
#include <thread.h>
#include <time.h>
#include <uart.h>
#include <bme280.h>
#include <ds3231.h>

using namespace std;

namespace
{
uint16_t co2 = 1201;
float temprature = 0.0f;
uint16_t pressure = 0;
uint8_t humidity = 0;
DateTime currentDateTime;
Uart<bsp::log::logPort>& log = *Uart<bsp::log::logPort>::instance();
Ds3231 ds(0x68);

void lcdThread(void*)
{
	Lcd lcd(0x27, 20, 4, 1);
	lcd.init();

	uint32_t count = 0;

	for (;;) {
		lcd.setCursor(0, 0);
		lcd.sendString(utils::stringFormat(
		    "%02u:%02u", currentDateTime.hour(), currentDateTime.minute(),
		    currentDateTime.second()));
		
		lcd.sendChar(' ');

		lcd.sendString(utils::stringFormat(
		    "%02u.%02u.%02u", currentDateTime.day(), currentDateTime.month(),
		    currentDateTime.year()));

		lcd.setCursor(0, 1);
		lcd.sendString(
		    utils::stringFormat("%s", utils::ftostring(temprature).c_str()));
		lcd.sendChar(0xDF);

		lcd.sendChar(' ');

		lcd.sendString(
		    utils::stringFormat("%u", pressure));
		lcd.sendString("mm");

		lcd.sendChar(' ');
		lcd.sendChar(' ');

		lcd.sendString(utils::stringFormat("%u", humidity));
		lcd.sendChar('%');
		Time::sleep(Time::secs(1));
	}
}

void blinkThread(void*)
{
	using led = GPIO<PinDef<CSP_GPIO_PORT_NBR_C, GPIO_PIN_13>, bsp::mOutputPP>;
	led::setup();

	for(;;) {
		led::on();
		Time::sleep(Time(500));
		led::off();
		Time::sleep(Time(500));
	}
}

void bmeThread(void*) 
{
	Bme280 bme(0x76);
	bme.init();

	bme.setSampling(Bme280::MODE_FORCED,
	                 Bme280::SAMPLING_X1,  // temperature
	                 Bme280::SAMPLING_X1,  // pressure
	                 Bme280::SAMPLING_X1,  // humidity
	                 Bme280::FILTER_OFF);

	for(;;) {
		bme.takeForcedMeasurement();
		float temp = bme.readTemperature();
		pressure = static_cast<uint16_t>(bme.readPressure() / 133.0f);
		humidity = static_cast<uint8_t>(bme.readHumidity());

		if (temp >= -99.0f)
			temprature = temp;

		Time::sleep(Time::secs(10));
	}
}

void dateThread(void*)
{

	for (;;) {
		currentDateTime = ds.now();
		Time::sleep(Time::secs(1));
	}
}

void uartThread(void*)
{
	string receiveMessage;

	for(;;) {
		receiveMessage = log.readln(1000);

		if (not receiveMessage.empty()) {
			if (receiveMessage.rfind("AT+SETDATETIME ", 0) == 0) {
				receiveMessage.erase(0, 15);
				log.send(utils::stringFormat("Set new date and time: %s",
				                             receiveMessage.c_str()));
				DateTime newDateTime = receiveMessage.c_str();
				ds.adjust(newDateTime);
				ds.adjust(newDateTime);
			}
		} 

		Time::sleep(Time(500));
	}
}
}

void applogic::startLogic()
{
	log.init(128, 115200);

	I2C<bsp::i2cP1>& i2c = *I2C<bsp::i2cP1>::instance();
	i2c.init();

	log.send("Start application");

	string result = "Addresses:";
	for (auto address : i2c.searchAddresses()) {
		result += utils::stringFormat(" %02X", address);
	}
	log.send(result);

	Thread::start(lcdThread, "lcd", 1024, 1);
	Thread::start(blinkThread, "blink", 512, 0);
	Thread::start(bmeThread, "bme", 1024, 2);
	Thread::start(dateThread, "date", 1024, 2);
	Thread::start(uartThread, "uart", 1024, 0);

	for (;;) {

		auto readMessage = log.readln(10);
		if (not readMessage.empty())
			log.send(
			    utils::stringFormat("Result read: %s\r", readMessage.c_str()));

        Time::sleep(Time::secs(1));
	}
}
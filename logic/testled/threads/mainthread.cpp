#include <mainthread.h>

/*
namespace Test
{
using rsPin = GPIO<PinDef<CSP_GPIO_PORT_NBR_A, GPIO_PIN_8>, mOutputPP>;
using ePin = GPIO<PinDef<CSP_GPIO_PORT_NBR_A, GPIO_PIN_9>, mOutputPP>;
using d4Pin = GPIO<PinDef<CSP_GPIO_PORT_NBR_B, GPIO_PIN_5>, mOutputPP>;
using d5Pin = GPIO<PinDef<CSP_GPIO_PORT_NBR_B, GPIO_PIN_4>, mOutputPP>;
using d6Pin = GPIO<PinDef<CSP_GPIO_PORT_NBR_B, GPIO_PIN_3>, mOutputPP>;
using d7Pin = GPIO<PinDef<CSP_GPIO_PORT_NBR_A, GPIO_PIN_15>, mOutputPP>;

void lcdSendByte(uint8_t data)
{
	if ((data & (1 << 0)) == 0) rsPin::off(); 
    else rsPin::on();

	if ((data & (1 << 2)) == 0) ePin::off();
	else ePin::on();

    if ((data & (1 << 4)) == 0) d4Pin::off();
	else d4Pin::on();

    if ((data & (1 << 5)) == 0) d5Pin::off();
	else d5Pin::on();

    if ((data & (1 << 6)) == 0) d6Pin::off();
	else d6Pin::on();

    if ((data & (1 << 7)) == 0) d7Pin::off();
	else d7Pin::on();
}
}
*/

MainThread::MainThread() : Thread(osPriorityNormal, 0, "main") {}

MainThread::~MainThread() {}

void MainThread::threadFunc()
{
	blink::setup();

	I2C<i2c::i2cPort1>::instance()->init();

	/*
	rsPin::setup();
	ePin::setup();
	d4Pin::setup();
	d5Pin::setup();
	d6Pin::setup();
	d7Pin::setup();
	*/

	Lcd i2cLcd(0x27, 16, 2, 1);
	//Lcd gpioLcd(20, 4, 1, lcdSendByte);

	i2cLcd.init();
	i2cLcd.setCursor(0, 1);
	i2cLcd.sendString("Test!!!!");

	/*
	gpioLcd.init();
	gpioLcd.setCursor(0, 1);
	gpioLcd.sendString("Test!!!!");
	*/

	for (;;) {
		blink::on();
		Time::sleep(Time(500));
		blink::off();
		Time::sleep(Time(500));
	}
}
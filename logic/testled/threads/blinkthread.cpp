#include <blinkthread.h>

using namespace gpio;

BlinkThread::BlinkThread() : Thread(osPriorityLow, 256, "blink") {}

void BlinkThread::threadFunc()
{
    using errorNormal = GPIO<PinDef<CSP_GPIO_PORT_NBR_C, GPIO_PIN_13>, mOutputPP>;
    errorNormal::setup();

	for(;;) {
		errorNormal::on();
		Time::sleep(Time(500));
		errorNormal::off();
		Time::sleep(Time(500));
	}
}
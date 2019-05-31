#pragma once

#include <thread.h>
#include <cmsis_os.h>
#include <gpio.h>
#include <time.h>
#include <lcd.h>

using namespace gpio;

class MainThread : public Thread
{
private:
	using blink = GPIO<PinDef<CSP_GPIO_PORT_NBR_C, GPIO_PIN_13>, mOutputPP>;

public:
    MainThread();
    virtual ~MainThread();

    void threadFunc() override;
};
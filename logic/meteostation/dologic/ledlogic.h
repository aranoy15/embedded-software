#pragma once

#include <singleton.h>
#include <gpio.h>
#include <timer.h>

using namespace gpio;

class LedLogic : public Singleton<LedLogic>
{
    using greenLed = GPIO<PinDef<CSP_GPIO_PORT_NBR_B, GPIO_PIN_12>, mOutputPP>;
    using redLed = GPIO<PinDef<CSP_GPIO_PORT_NBR_B, GPIO_PIN_13>, mOutputPP>;
    using blueLed = GPIO<PinDef<CSP_GPIO_PORT_NBR_B, GPIO_PIN_14>, mOutputPP>;

    void good();
    void normal();
    void bad();

    Timer mTimer;


public:
    LedLogic();

    void processLed();
};
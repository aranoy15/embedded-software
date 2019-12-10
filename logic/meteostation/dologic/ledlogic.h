#pragma once

#include <singleton.h>
#include <timer.h>
#include <bsp.h>

using namespace bsp;

class LedLogic : public Singleton<LedLogic>
{
    using greenLed = GPIO<PinDef<CSP_GPIO_PORT_NBR_B, GPIO_PIN_13>, mOutputPP>;
    using redLed = GPIO<PinDef<CSP_GPIO_PORT_NBR_B, GPIO_PIN_14>, mOutputPP>;
    using blueLed = GPIO<PinDef<CSP_GPIO_PORT_NBR_B, GPIO_PIN_12>, mOutputPP>;

    void good();
    void normal();
    void bad();

    Timer mTimer;


public:
    LedLogic();

    void processLed();
};
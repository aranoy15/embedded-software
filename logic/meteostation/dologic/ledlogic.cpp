#include <ledlogic.h>
#include <sensorlogic.h>

LedLogic::LedLogic() : mTimer(1000)
{
    redLed::setup();
    blueLed::setup();
    greenLed::setup();
}

void LedLogic::good()
{
    redLed::off();
    blueLed::off();
    greenLed::on();
}

void LedLogic::normal()
{
    redLed::off();
    blueLed::on();
    greenLed::off();
}

void LedLogic::bad()
{
    redLed::on();
    blueLed::off();
    greenLed::off();
}

void LedLogic::processLed()
{
    if (mTimer.elapsed()) {
        uint16_t co2 = SensorLogic::instance()->getCO2();

        if (co2 < 800) good();
        else if (co2 >= 800 and co2 < 1200) normal();
        else bad();

        mTimer.start();
    }
}

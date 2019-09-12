#pragma once

#include <button.h>
#include <singleton.h>

using namespace bsp;

enum class LogicStateType
{
    MainInfo = 0,
    Co2HourChart = 1,
    Co2DayChart = 2,
    TempHourChart = 3,
    TempDayChart = 4,
    HumidityHourChart = 5,
    HumidityDayChart = 6,
    First = MainInfo,
    Last = HumidityDayChart
};

class LogicState : public Singleton<LogicState>
{
    using button = Button<GPIO<PinDef<CSP_GPIO_PORT_NBR_B, GPIO_PIN_11>, mInput>>;

private:
    uint8_t mCurrentState;

public:
	LogicState();

    void init();
    void process();

    LogicStateType currentState() const { return static_cast<LogicStateType>(mCurrentState); }
};
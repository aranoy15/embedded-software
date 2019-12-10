#pragma once

#include <taskscheduler.h>
#include <i2cds18b20.h>
#include <ads7844.h>
#include <i2cds18b20async.h>

class SensorTask : public TaskBase
{
	Ds18b20 mTempSource;
	Ads7844 mAnalogInput;

	Ds18b20Async _evaporator_sensor;
	Ds18b20Async _condensator_sensor;

	uint8_t _current_sensor;

	Timer _switch_timer;

public:
    SensorTask();
    virtual ~SensorTask();

	void setup() override;
	void func() override;
};
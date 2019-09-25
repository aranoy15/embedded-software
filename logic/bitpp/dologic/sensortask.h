#pragma once

#include <taskscheduler.h>
#include <i2cds18b20.h>
#include <ads7844.h>

class SensorTask : public TaskBase
{
	Ds18b20 mTempSource;
	Ads7844 mAnalogInput;

public:
    SensorTask();
    virtual ~SensorTask();

	void setup() override;
	void func() override;
};
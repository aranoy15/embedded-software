#pragma once

#include <bsp.h>
#include <i2conewirebase.h>
#include <time.h>
#include <timer.h>
#include <type_traits>

class Ds18b20Async : public I2COnewireBase
{
private:
    int16_t _value;
    uint16_t _channel;
    Timer _timer;

    const uint16_t _timeout = 750;

    void init_channel();

public:
	Ds18b20Async(uint16_t channel);

    void update();

	float value()
	{
        return _value / 16.0f;
	}
};
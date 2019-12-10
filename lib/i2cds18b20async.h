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
    uint8_t _channel;
    Timer _timer;

public:
	Ds18b20Async(uint8_t channel);

    bool update();

	template <typename T>
	auto value() -> T
	{
        if (std::is_same<T, float>::value)
            return static_cast<float>(_value) / 16.0f;

        return _value;
	}
};
#pragma once

#include <bsp.h>
#include <i2conewirebase.h>
#include <time.h>

class Ds18b20 : public I2COnewireBase
{
public:
	Ds18b20();

    std::tuple<bool, int16_t> update(uint8_t, int = 1);

    static float calcValue(int16_t);
};
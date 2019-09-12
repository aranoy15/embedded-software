#pragma once

#include <bsp.h>
#include <i2conewirebase.h>
#include <time.h>

class Ds18b20 : public I2COnewireBase
{
public:
	Ds18b20();

    //virtual std::tuple<bool, float> getTemp(int count) override;
    std::tuple<bool, uint16_t> update(uint8_t, int = 1) override;

    static float calcValue(uint16_t);
};
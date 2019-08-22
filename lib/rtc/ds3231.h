#pragma once

#include <inttypes.h>
#include <datetime.h>
#include <i2c.h>
#include <bsp.h>

class Ds3231
{
private:
	enum Ds3231SqwPinMode
	{
		DS3231_OFF = 0x01,
		DS3231_SquareWave1Hz = 0x00,
		DS3231_SquareWave1kHz = 0x08,
		DS3231_SquareWave4kHz = 0x10,
		DS3231_SquareWave8kHz = 0x18
	};

    static uint8_t bcd2bin (uint8_t val) { return val - 6 * (val >> 4); }
    static uint8_t bin2bcd (uint8_t val) { return val + 6 * (val / 10); }

private:
    uint16_t m_address;
	I2C<bsp::i2crtc::ds3231Port>& getI2C() { return *I2C<bsp::i2crtc::ds3231Port>::instance(); }

private:
    static const uint8_t CONTROL = 0x0E;
    static const uint8_t STATUSREG = 0x0F;

public:
    Ds3231(uint16_t address);

    void adjust(const DateTime& dateTime);
    bool lostPower();
    DateTime now();
    Ds3231SqwPinMode readSqwPinMode();
    void writeSqwPinMode(Ds3231SqwPinMode mode);
};
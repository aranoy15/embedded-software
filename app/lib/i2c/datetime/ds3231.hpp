#ifndef DRIVERS_LIB_I2C_DATETIME_DS3231
#define DRIVERS_LIB_I2C_DATETIME_DS3231

#include <cinttypes>
#include <lib/datetime/datetime.hpp>
#include <lib/i2c/i2c.hpp>
#include <bsp.hpp>

namespace lib::i2c::datetime
{

class Ds3231
{
public:
    using datetime_t = lib::datetime::DateTime;
    using i2c_t = lib::i2c::I2C<bsp::i2c::ds3231_port>;

    enum Ds3231SqwPinMode
	{
		DS3231_OFF = 0x01,
		DS3231_SquareWave1Hz = 0x00,
		DS3231_SquareWave1kHz = 0x08,
		DS3231_SquareWave4kHz = 0x10,
		DS3231_SquareWave8kHz = 0x18
	};

public:
    Ds3231();

    void adjust(const datetime_t& dateTime);
    bool lost_power();
    datetime_t now();
    Ds3231SqwPinMode read_sqw_pin_mode();
    void write_sqw_pin_mode(Ds3231SqwPinMode mode);

private:
    static uint8_t bcd_to_bin (uint8_t val) { return val - 6 * (val >> 4); }
    static uint8_t bin_to_bcd (uint8_t val) { return val + 6 * (val / 10); }

private:
    std::uint16_t _address = bsp::i2c::ds3231_address;
    bsp::i2c::port_t _port = bsp::i2c::ds3231_port;

private:
    static const uint8_t CONTROL = 0x0E;
    static const uint8_t STATUSREG = 0x0F;
};
}

#endif /* DRIVERS_LIB_I2C_DATETIME_DS3231 */

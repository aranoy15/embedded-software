#include <lib/i2c/datetime/ds3231.hpp>

using namespace lib::i2c::datetime;

Ds3231::Ds3231() {}

bool Ds3231::lost_power()
{
    return bool(i2c_t::receive_reg<uint8_t, uint8_t>(_address, STATUSREG));
}

void Ds3231::adjust(const datetime_t& dateTime)
{
	uint8_t master[] = {0,
	                    bin_to_bcd(dateTime.second()),
	                    bin_to_bcd(dateTime.minute()),
	                    bin_to_bcd(dateTime.hour()),
	                    bin_to_bcd(0),
	                    bin_to_bcd(dateTime.day()),
	                    bin_to_bcd(dateTime.month()),
	                    bin_to_bcd(dateTime.year() - 2000)};

    i2c_t::send(_address, master, sizeof(master));

	uint8_t statreg = i2c_t::receive_reg<uint8_t, uint8_t>(_address, STATUSREG);
	statreg &= ~0x80;  // flip OSF bit
	i2c_t::send_reg<uint8_t, uint8_t>(_address, STATUSREG, statreg);
}

auto Ds3231::now() -> datetime_t
{
	i2c_t::send_byte(_address, 0);

	uint8_t ss = bcd_to_bin(i2c_t::receive_byte(_address) & 0x7F);
	uint8_t mm = bcd_to_bin(i2c_t::receive_byte(_address));
	uint8_t hh = bcd_to_bin(i2c_t::receive_byte(_address));
	i2c_t::receive_byte(_address);
	uint8_t d = bcd_to_bin(i2c_t::receive_byte(_address));
	uint8_t m = bcd_to_bin(i2c_t::receive_byte(_address));
	uint16_t y = bcd_to_bin(i2c_t::receive_byte(_address)) + 2000;

	return datetime_t(y, m, d, hh, mm, ss);
}
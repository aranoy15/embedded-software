#include <ds3231.h>

Ds3231::Ds3231(uint16_t address) : m_address(address) {}

bool Ds3231::lostPower()
{
    return bool(getI2C().readReg<uint8_t, uint8_t>(m_address, STATUSREG));
}

void Ds3231::adjust(const DateTime& dateTime)
{
	uint8_t master[] = {0,
	                    bin2bcd(dateTime.second()),
	                    bin2bcd(dateTime.minute()),
	                    bin2bcd(dateTime.hour()),
	                    bin2bcd(0),
	                    bin2bcd(dateTime.day()),
	                    bin2bcd(dateTime.month()),
	                    bin2bcd(dateTime.year() - 2000)};

    getI2C().write(m_address, master, sizeof(master));

	uint8_t statreg = getI2C().readReg<uint8_t, uint8_t>(m_address, STATUSREG);
	statreg &= ~0x80;  // flip OSF bit
	getI2C().writeReg<uint8_t, uint8_t>(m_address, STATUSREG, statreg);
}

DateTime Ds3231::now()
{
	getI2C().writeByte(m_address, 0);

	uint8_t ss = bcd2bin(getI2C().readByte(m_address) & 0x7F);
	uint8_t mm = bcd2bin(getI2C().readByte(m_address));
	uint8_t hh = bcd2bin(getI2C().readByte(m_address));
	getI2C().readByte(m_address);
	uint8_t d = bcd2bin(getI2C().readByte(m_address));
	uint8_t m = bcd2bin(getI2C().readByte(m_address));
	uint16_t y = bcd2bin(getI2C().readByte(m_address)) + 2000;

	return DateTime(y, m, d, hh, mm, ss);
}
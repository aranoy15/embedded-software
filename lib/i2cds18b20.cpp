#include <i2cds18b20.h>
#include <tuple>

Ds18b20::Ds18b20() : I2COnewireBase() {}

std::tuple<bool, uint16_t> Ds18b20::update(uint8_t channel, int count)
{
	bool result = false;
	uint16_t resultData = 0;

	channelSelect(channel);

    uint8_t addr[8];
	uint8_t data[9];

	if (count == 1 || wireSearch(addr))
	{
		wireReset();
		if (count == 1) wireSkip(); else wireSelect(addr);
		wireWriteByte(0x44);
		Time::sleep(Time::msecs(750));
		wireReset();
		if (count == 1) wireSkip(); else wireSelect(addr);
		wireWriteByte(0xbe);
		for (int i = 0; i < 9; i++)
			data[i] = wireReadByte();

		if (crc8(data, 8) == data[8])
		{
			int16_t raw = (data[1] << 8) | data[0];
			
			if (data[4] == 0xFF)
				raw = raw << 3; // 9 bit resolution default
			if (data[7] == 0x10) 
			{
				// "count remain" gives full 12 bit resolution
				raw = (raw & 0xFFF0) + 12 - data[6];
			}

		 	resultData = raw;
			result = true;
		}
	}

	return std::make_tuple(result, resultData);
}

float Ds18b20::calcValue(uint16_t value)
{
	//return static_cast<float>(value) / 16.0f;
	float result = 0.0f;;

    if (value & 0x800)
        result = (value - 0xFFF) / 16.0f;
    else
        result = value / 16.0f;

    return result;
}
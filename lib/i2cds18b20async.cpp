#include <i2cds18b20async.h>

Ds18b20Async::Ds18b20Async(uint8_t channel)
    : _value(0), _channel(channel), _timer(750)
{
}

bool Ds18b20Async::update()
{
    bool result = false;

    if (not _timer.is_started()) {
        channelSelect(_channel);

        wireReset();
        wireSkip();
        wireWriteByte(0x44);

        _timer.start();
    } else if (_timer.elapsed()) {
        uint8_t data[9];

        //channelSelect(_channel);

        wireReset();
        wireSkip();
        wireWriteByte(0xBE);

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

		 	_value = raw;
			result = true;
		}

        _timer.stop();
    }

    return result;
}
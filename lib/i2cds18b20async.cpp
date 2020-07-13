#include <i2cds18b20async.h>

Ds18b20Async::Ds18b20Async(uint16_t channel)
    : _value(0), _channel(channel), _timer()
{
}

void Ds18b20Async::init_channel()
{
    wireReset();
	channelSelect(_channel);
	wireSkip();
	wireWriteByte(0x44);
}

void Ds18b20Async::update()
{
    if (not _timer.is_started()) {
        init_channel();
        _timer.start();
	} else {
		uint8_t data[9];

		wireReset();
		channelSelect(_channel);
		wireSkip();
		wireWriteByte(0xBE);

		for (int i = 0; i < 9; i++)
			data[i] = wireReadByte();

		if (crc8(data, 8) == data[8]) {
			int16_t raw = (data[1] << 8) | data[0];

			if (data[4] == 0xFF) raw = raw << 3;  // 9 bit resolution default
			if (data[7] == 0x10) {
				// "count remain" gives full 12 bit resolution
				raw = (raw & 0xFFF0) + 12 - data[6];
			}

			_value = raw;
		}

        _timer.stop();
	}
}
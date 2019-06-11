#include <mhz19.h>

Mhz19::Mhz19()
{
    //getUart().init(32, 9600);
}


uint16_t Mhz19::getCO2()
{
    uint16_t result = 0;

    uint8_t buffer[9] = {0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    buffer[8] = calcCrc(buffer);
    getUart().send(buffer, 9);

	if (getUart().readCount(buffer, 9, 1000)) {
		result = (buffer[2] << 8) | buffer[3];
	}


	return result;
}

uint8_t Mhz19::calcCrc(uint8_t data[])
{
    uint8_t crc = 0;

    for (uint8_t i = 1; i < 8; i++) {
        crc += data[i];
    }

    crc = 255 - crc;
    crc++;

    return crc;
}
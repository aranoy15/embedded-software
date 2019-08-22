#include <mhz19.h>

Mhz19Data::Mhz19Data() : co2(0), temperature(0), status(0) {}

Mhz19::Mhz19() : buffer()
{
}

void Mhz19::init()
{
    getUart().init(32, 9600);
}

uint16_t Mhz19::getCO2()
{
    uint16_t result = 0;
    command(0x86, static_cast<uint8_t>(0));

    if (readResponse()) {
        result = (buffer[2] << 8) | buffer[3];
    }

	return result;
}

void Mhz19::command(uint8_t command, uint8_t b3, uint8_t b4, uint8_t b5, uint8_t b6, uint8_t b7)
{
    buffer[0] = 0xFF;
    buffer[1] = 0x01;
    buffer[2] = command;
    buffer[3] = b3;
    buffer[4] = b4;
    buffer[5] = b5;
    buffer[6] = b6;
    buffer[7] = b7;
    buffer[8] = calcCrc();

    getUart().send(buffer, bufferSize);
}

bool Mhz19::readResponse()
{
    bool result = getUart().readCount(buffer, bufferSize, 100);

    if (result)
        result = (buffer[8] == calcCrc());

    return result;
}



void Mhz19::setAutoCalibration(bool state)
{
    command(OnOffAutoCalibration, state ? 0xA0 : 0x00);
}


uint8_t Mhz19::calcCrc()
{
    uint8_t crc = 0;

    for (uint8_t i = 1; i < bufferSize - 1; i++) {
        crc += buffer[i];
    }

    crc = 255 - crc;
    crc++;

    return crc;
}

void Mhz19::setRange(Mhz19Range range)
{
    switch (range) {
		case Mhz19Range::Range1000:
			command(DetectionRangeSetting, 0x00, 0x00, 0x00, 0x03, 0xE8);
			break;
		case Mhz19Range::Range2000:
			command(DetectionRangeSetting, 0x00, 0x00, 0x00, 0x07, 0xD0);
			break;
		case Mhz19Range::Range3000:
			command(DetectionRangeSetting, 0x00, 0x00, 0x00, 0x0B, 0xB8);
			break;
		case Mhz19Range::Range5000:
			command(DetectionRangeSetting, 0x00, 0x00, 0x00, 0x13, 0x88);
 			break;
		case Mhz19Range::Range10000:
			command(DetectionRangeSetting, 0x00, 0x00, 0x00, 0x27, 0x10);
 			break;
		default:
			break;
	}
}
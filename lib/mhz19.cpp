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

void Mhz19::command(uint8_t command, uint8_t high, uint8_t low)
{
    buffer[0] = 0xFF;
    buffer[1] = 0x01;
    buffer[2] = command;
    buffer[3] = high;
    buffer[4] = low;
    buffer[8] = calcCrc();

    //getLog().dump(buffer, bufferSize, "Command data: ");
    getUart().send(buffer, bufferSize);
}

bool Mhz19::readResponse()
{
    bool result = getUart().readCount(buffer, bufferSize, 100);
    //getLog().dump(buffer, bufferSize, "Response data: ");

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
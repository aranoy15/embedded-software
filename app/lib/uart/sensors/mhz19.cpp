#include <lib/uart/sensors/mhz19.hpp>
#include <lib/time/time.hpp>

using namespace lib::uart::sensors;

Mhz19Data::Mhz19Data() : co2(0), temperature(0), status(0) {}

Mhz19::Mhz19() : _buffer()
{
}

void Mhz19::init()
{
}

uint16_t Mhz19::get_co2()
{
    uint16_t result = 0;
    uart_t::clear();

    command(0x86, static_cast<uint8_t>(0));

    //lib::time::Time::sleep(lib::time::Time());

    if (read_response()) {
        result = (_buffer[2] << 8) | _buffer[3];
    }

	return result;
}

void Mhz19::command(uint8_t command, uint8_t b3, uint8_t b4, uint8_t b5, uint8_t b6, uint8_t b7)
{
    _buffer[0] = 0xFF;
    _buffer[1] = 0x01;
    _buffer[2] = command;
    _buffer[3] = b3;
    _buffer[4] = b4;
    _buffer[5] = b5;
    _buffer[6] = b6;
    _buffer[7] = b7;
    _buffer[8] = calc_crc();

    //getUart().send(buffer, bufferSize);
    uart_t::send(_buffer, _buffer_size);
}

bool Mhz19::read_response()
{
    bool result = uart_t::read(_buffer, _buffer_size);

    if (result)
        result = (_buffer[8] == calc_crc());

    return result;
}



void Mhz19::set_auto_calibration(bool state)
{
    command(OnOffAutoCalibration, state ? 0xA0 : 0x00);
}


uint8_t Mhz19::calc_crc()
{
    uint8_t crc = 0;

    for (uint8_t i = 1; i < _buffer_size - 1; i++) {
        crc += _buffer[i];
    }

    crc = 255 - crc;
    crc++;

    return crc;
}

void Mhz19::set_range(Mhz19Range range)
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
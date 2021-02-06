#ifndef DRIVERS_LIB_UART_SENSORS_MHZ19
#define DRIVERS_LIB_UART_SENSORS_MHZ19

#include <lib/uart/uart.hpp>
#include <vector>
#include <bsp.hpp>

namespace lib::uart::sensors
{

enum class Mhz19Range {
	Range1000,
	Range2000,
	Range3000,
	Range5000,
	Range10000	
};

struct Mhz19Data
{
	int co2;
	int temperature;
	int status;

	Mhz19Data();
};

class Mhz19
{
public:
	using uart_t = lib::uart::Uart<bsp::usart::mhz_port>;

private:
	static const uint8_t _buffer_size = 9;
	uint8_t _buffer[_buffer_size];

	enum Commands
	{
		ReadCO2Concetration = 0x86,
		CalibrateZeroPoint = 0x87,
		CalibrateSpanPoint = 0x88,
		OnOffAutoCalibration = 0x79,
		DetectionRangeSetting = 0x99
	};

public:
    Mhz19();

	void init();
    uint16_t get_co2();
	void set_auto_calibration(bool);
	void set_range(Mhz19Range);

private:
	void command(uint8_t, uint8_t = 0, uint8_t = 0, uint8_t = 0, uint8_t = 0, uint8_t = 0);
	bool read_response();

    uint8_t calc_crc();
};

}

#endif /* DRIVERS_LIB_UART_SENSORS_MHZ19 */

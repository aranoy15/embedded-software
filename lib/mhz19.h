#pragma once

#include <uart.h>
#include <vector>

struct Mhz19Data
{
	int co2;
	int temperature;
	int status;

	Mhz19Data();
};

class Mhz19
{
private:
	Uart<uart::UartPort::usart2>& getUart()
	{
		return *Uart<uart::UartPort::usart2>::instance();
	}

	Uart<uart::UartPort::usart1>& getLog()
	{
		return *Uart<uart::UartPort::usart1>::instance();
	}

	static const uint8_t bufferSize = 9;
	uint8_t buffer[bufferSize];

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
    uint16_t getCO2();
	void setAutoCalibration(bool);

private:
	void command(uint8_t, uint8_t = 0, uint8_t = 0);
	bool readResponse();

    uint8_t calcCrc();
};
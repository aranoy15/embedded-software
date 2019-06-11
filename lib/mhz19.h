#pragma once

#include <uart.h>
#include <vector>

class Mhz19
{
private:
	Uart<uart::UartPort::usart2>& getUart()
	{
		return *Uart<uart::UartPort::usart2>::instance();
	}

public:
    Mhz19();

    uint16_t getCO2();

private:
    uint8_t calcCrc(uint8_t data[]);
};
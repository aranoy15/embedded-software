#pragma once

#include <spi.h>
#include <bsp.h>
#include <timer.h>

class Ads7844
{
private:
	using spiAdc = Spi<bsp::adc::port>;
	using adcChipSelectTrigger = ChipSelectTrigger<bsp::adc::port>;

	spiAdc& getSpi() const { return *spiAdc::instance(); }

private:
	static const uint8_t shift = 4;
	static const uint8_t channelCount = 8;
	static const uint8_t channels[channelCount];

	Timer m_timer;

	enum
	{
		cbPD0 = 1 << 0,
		cbPD1 = 1 << 1,
		cbSGLDIF = 1 << 2,
		cbStart = 1 << 7
	};

	//void chipSelect();
	//void chipUnselect();

public:
	Ads7844();

	uint16_t getValue(uint8_t);

	static float calcValue(uint16_t);
};
#pragma once

#include <csp.h>
#include <bsp_template.h>

namespace bsp
{

#if (USE_FREERTOS)
	void initMem();
#endif

namespace uart
{
	using usart1TxPin =
	    GPIO<PinDef<CSP_GPIO_PORT_NBR_A, GPIO_PIN_9>, mAfPP, sHi, pUp, GPIO_AF7_USART1>;
	using usart1RxPin =
	    GPIO<PinDef<CSP_GPIO_PORT_NBR_A, GPIO_PIN_10>, mAfPP, sHi, pUp, GPIO_AF7_USART1>;

	using usart2TxPin =
	    GPIO<PinDef<CSP_GPIO_PORT_NBR_A, GPIO_PIN_2>, mAfPP, sHi, pUp, GPIO_AF7_USART2>;
	using usart2RxPin =
	    GPIO<PinDef<CSP_GPIO_PORT_NBR_A, GPIO_PIN_3>, mAfPP, sHi, pUp, GPIO_AF7_USART2>;

	void usartInitGpio(UartPort port);
	USART_TypeDef* port2CSP(UartPort port);

}  // namespace uart

namespace log
{
	static const UartPort logPort = uartP1;
}

namespace mhz19uart
{
	static const UartPort mhzPort = uartP2;
}

namespace i2c
{
	using i2cPins = GPIO<PinDef<CSP_GPIO_PORT_NBR_B, GPIO_PIN_6 | GPIO_PIN_7>,
	                     mAfOD, sHi, pUp, GPIO_AF4_I2C1>;

	void init(bsp::I2CPort);
	I2C_TypeDef* port2CSP(bsp::I2CPort);
	
}  // namespace i2c

namespace lcd
{
	static const bsp::I2CPort lcdI2CPort = bsp::i2cP1;
}

namespace i2ctemp
{
	static const bsp::I2CPort bmeI2CPort = bsp::i2cP1;
}

namespace i2crtc
{
	static const bsp::I2CPort ds3231Port = bsp::i2cP1;
}

//spi
namespace spi
{
	using adcSck = GPIO<PinDef<CSP_GPIO_PORT_NBR_A, GPIO_PIN_5>, mAfPP, sHi>;
	using adcMiso = GPIO<PinDef<CSP_GPIO_PORT_NBR_A, GPIO_PIN_6>, mInput>;
	using adcMosi = GPIO<PinDef<CSP_GPIO_PORT_NBR_A, GPIO_PIN_7>, mAfPP, sHi>;

	using adcCsPin = GPIO<PinDef<CSP_GPIO_PORT_NBR_A, GPIO_PIN_4>, mOutputPP>;
	using adcCs = Relay<adcCsPin, PinManageLogic::Inverse>;

	void init(SpiPort port);
	void chipSelect(SpiPort port);
	void chipUnselect(SpiPort port);

	SPI_TypeDef* port2CSP(bsp::SpiPort);
}  // namespace spi
}  // namespace bsp
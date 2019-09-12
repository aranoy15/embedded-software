#pragma once

#include <csp.h>

#if (USE_FREERTOS)
	#include <FreeRTOS.h>
	#include <task.h>
	#include <semphr.h>
#endif

namespace bsp
{

#define O_ASSERT(x)                                                            \
	while (not x) {                                                            \
	}

enum UartPort
{
	uartLP1 = 0,
	uartP1 = 1,
	uartP2 = 2,
	uartP3 = 3,
	uartP4 = 4,
	uartP5 = 5,
};

enum ADCPort
{
	adcP1 = 1,
	adcP2 = 2,
	adcP3 = 3,
};


enum I2CPort
{
	i2cP1 = 1,
	i2cP2 = 2,
	i2cP3 = 3,
};


enum SpiPort
{
	spiP1 = 1,
	spiP2 = 2,
	spiP3 = 3,
};

enum TimerPort
{
	timerP2 = 2,
	timerP5 = 5
};

enum Mode
{
	mInput = GPIO_MODE_INPUT,
	mOutputPP = GPIO_MODE_OUTPUT_PP,
	mOutputOD = GPIO_MODE_OUTPUT_OD,
	mAfPP = GPIO_MODE_AF_PP,
	mAfOD = GPIO_MODE_AF_OD,
	mAnalog = GPIO_MODE_ANALOG,
	#if defined F4
	mAnalogADCCtrl = GPIO_MODE_ANALOG_ADC_CONTROL,
	#endif
	mITRising = GPIO_MODE_IT_RISING,
	mITFalling = GPIO_MODE_IT_FALLING,
	mITRisingFalling = GPIO_MODE_IT_RISING_FALLING,
	mEVTRising = GPIO_MODE_EVT_RISING,
	mEVTFalling = GPIO_MODE_EVT_FALLING,
	mEVTRisingFalling = GPIO_MODE_EVT_RISING_FALLING,
};

enum Speed
{
	sLow = GPIO_SPEED_FREQ_LOW,
	sMed = GPIO_SPEED_FREQ_MEDIUM,
	sHi = GPIO_SPEED_FREQ_HIGH,

	#if defined F4
	sVHi = GPIO_SPEED_FREQ_VERY_HIGH,
	#endif
};


enum Pull
{
	pNo = GPIO_NOPULL,
	pUp = GPIO_PULLUP,
	pDown = GPIO_PULLDOWN,
};

void cpuInit();

void init();

}

template <CSP_DEV_NBR portNumber, CSP_GPIO_MSK pinBits>
struct PinDef
{
	enum
	{
		Port = portNumber
	};
	enum
	{
		Pin = pinBits
	};
};

GPIO_TypeDef *GPIO_GetPointer(CSP_DEV_NBR portNum);
void setupGpio(CSP_DEV_NBR, CSP_GPIO_MSK, bsp::Mode = bsp::mInput, bsp::Speed = bsp::sLow, bsp::Pull = bsp::pNo);

template <class PinConfig, bsp::Mode mode = bsp::mInput, bsp::Speed speed = bsp::sHi,
          bsp::Pull pull = bsp::pNo>
struct GPIO
{
	typedef PinConfig Pins;

	static void setup()
	{
		setupGpio(PinConfig::Port, PinConfig::Pin, mode, speed, pull);
	}

	static void on()
	{
		//StaticAssert<mode == mOutputPP or mode == mOutputOD>();
		HAL_GPIO_WritePin(GPIO_GetPointer(PinConfig::Port), PinConfig::Pin,
		                  GPIO_PIN_SET);
	}

	static void off()
	{
		//StaticAssert<mode == mOutputPP or mode == mOutputOD>();
		HAL_GPIO_WritePin(GPIO_GetPointer(PinConfig::Port), PinConfig::Pin,
		                  GPIO_PIN_RESET);
	}

	static bool state()
	{
		return HAL_GPIO_ReadPin(GPIO_GetPointer(PinConfig::Port),
		                        PinConfig::Pin);
	}

	static void remove()
	{
		HAL_GPIO_DeInit(GPIO_GetPointer(PinConfig::Port), PinConfig::Pin);
	}
};

enum class PinManageLogic 
{
	Inverse = true,
	Straight = false
};

template<class Pin, PinManageLogic = PinManageLogic::Straight>
struct Relay
{
	static void setup() { Pin::setup(); }
	static void on() { Pin::on(); }
	static void off() { Pin::off(); }
	static bool state() { return Pin::state(); }
};

template<class Pin>
struct Relay<Pin, PinManageLogic::Inverse>
{
	static void setup() { Pin::setup(); }
	static void on() { Pin::off(); }
	static void off() { Pin::on(); }
	static bool state() { return not Pin::state(); }
};
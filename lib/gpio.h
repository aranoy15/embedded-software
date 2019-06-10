#pragma once

#include <stdint.h>
#include <stm32f1xx_hal.h>

typedef uint16_t CSP_DEV_NBR;
typedef uint32_t CSP_GPIO_MSK;

#define CSP_GPIO_PORT_NBR_A (CSP_DEV_NBR)(0u)
#define CSP_GPIO_PORT_NBR_B (CSP_DEV_NBR)(1u)
#define CSP_GPIO_PORT_NBR_C (CSP_DEV_NBR)(2u)
#define CSP_GPIO_PORT_NBR_D (CSP_DEV_NBR)(3u)
#define CSP_GPIO_PORT_NBR_E (CSP_DEV_NBR)(4u)

namespace gpio
{
#define O_ASSERT(x)                                                            \
	while (not x) {                                                            \
	}

enum Mode
{
	mInput = GPIO_MODE_INPUT,
	mOutputPP = GPIO_MODE_OUTPUT_PP,
	mOutputOD = GPIO_MODE_OUTPUT_OD,
	mAfPP = GPIO_MODE_AF_PP,
	mAfOD = GPIO_MODE_AF_OD,
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
};

enum Pull
{
	pNo = GPIO_NOPULL,
	pUp = GPIO_PULLUP,
	pDown = GPIO_PULLDOWN,
};

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
void Setup(CSP_DEV_NBR, CSP_GPIO_MSK, Mode = mInput, Speed = sLow, Pull = pNo);

template <class PinConfig, Mode mode = mInput, Speed speed = sHi,
          Pull pull = pNo>
struct GPIO
{
	typedef PinConfig Pins;

	static void setup()
	{
		gpio::Setup(PinConfig::Port, PinConfig::Pin, mode, speed, pull);
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
}  // namespace gpio

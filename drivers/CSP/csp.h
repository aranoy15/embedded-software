#pragma once

#include <stdint.h>

#if defined F1
    #include <stm32f1xx_hal.h>
#endif

using CSP_GPIO_MSK = uint32_t;
using CSP_DEV_NBR = uint16_t;

#define CSP_GPIO_PORT_NBR_A (CSP_DEV_NBR)(0u)
#define CSP_GPIO_PORT_NBR_B (CSP_DEV_NBR)(1u)
#define CSP_GPIO_PORT_NBR_C (CSP_DEV_NBR)(2u)
#define CSP_GPIO_PORT_NBR_D (CSP_DEV_NBR)(3u)
#define CSP_GPIO_PORT_NBR_E (CSP_DEV_NBR)(4u)

/*
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
*/
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

void GpioSetup(CSP_DEV_NBR port, CSP_GPIO_MSK pins, uint32_t mode, uint16_t speed,
                 uint32_t pull);
GPIO_TypeDef *GpioGetPointer(CSP_DEV_NBR portNum);

void GpioOn(CSP_DEV_NBR port, CSP_GPIO_MSK pin);
void GpioOff(CSP_DEV_NBR port, CSP_GPIO_MSK pin);
uint8_t GpioState(CSP_DEV_NBR port, CSP_GPIO_MSK pin);
void GpioRemove(CSP_DEV_NBR port, CSP_GPIO_MSK pin);
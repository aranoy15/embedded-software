#include <gpio.h>

GPIO_TypeDef *gpio::GPIO_GetPointer(CSP_DEV_NBR portNum)
{
	switch (portNum) {
		case CSP_GPIO_PORT_NBR_A:
			if (__HAL_RCC_GPIOA_IS_CLK_DISABLED()) __HAL_RCC_GPIOA_CLK_ENABLE();
			return GPIOA;
		case CSP_GPIO_PORT_NBR_B:
			if (__HAL_RCC_GPIOB_IS_CLK_DISABLED()) __HAL_RCC_GPIOB_CLK_ENABLE();
			return GPIOB;
		case CSP_GPIO_PORT_NBR_C:
			if (__HAL_RCC_GPIOC_IS_CLK_DISABLED()) __HAL_RCC_GPIOC_CLK_ENABLE();
			return GPIOC;
		case CSP_GPIO_PORT_NBR_D:
			if (__HAL_RCC_GPIOD_IS_CLK_DISABLED()) __HAL_RCC_GPIOD_CLK_ENABLE();
			return GPIOD;
		case CSP_GPIO_PORT_NBR_E:
			if (__HAL_RCC_GPIOE_IS_CLK_DISABLED()) __HAL_RCC_GPIOE_CLK_ENABLE();
			return GPIOE;
	}

	return NULL;
}

void gpio::Setup(CSP_DEV_NBR port, CSP_GPIO_MSK pins, Mode mode, Speed speed,
                 Pull pull)
{
    GPIO_InitTypeDef cfg;
    cfg.Pin = pins;
    cfg.Mode = mode;
    cfg.Pull = pull;
    cfg.Speed = speed;

    HAL_GPIO_Init(GPIO_GetPointer(port), &cfg);
}
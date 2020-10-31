#include <drivers/bsp/F103xB_METEOSTATION/bsp.hpp>
#include <main.h>
#include <gpio.h>

extern "C" {
void SystemClock_Config(void);
}


void bsp::init()
{
    HAL_Init();

    SystemClock_Config();
}

void bsp::gpio::init()
{
    MX_GPIO_Init();
}

void bsp::gpio::status_on()
{
    HAL_GPIO_WritePin(GPIO_STATUS_GPIO_Port, GPIO_STATUS_Pin, GPIO_PIN_SET);
}

void bsp::gpio::status_off()
{
    HAL_GPIO_WritePin(GPIO_STATUS_GPIO_Port, GPIO_STATUS_Pin, GPIO_PIN_RESET);
}

uint32_t bsp::tick::current()
{
    return HAL_GetTick();
}

void bsp::tick::delay(uint32_t ms)
{
    HAL_Delay(ms);
}

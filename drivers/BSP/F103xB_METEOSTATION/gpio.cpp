#include <drivers/bsp/F103xB_METEOSTATION/gpio.hpp>
#include <gpio.h>

void bsp::gpio::init()
{
    MX_GPIO_Init();
}

void bsp::gpio::status::on()
{
    HAL_GPIO_WritePin(GPIO_STATUS_GPIO_Port, GPIO_STATUS_Pin, GPIO_PIN_SET);
}

void bsp::gpio::status::off()
{
    HAL_GPIO_WritePin(GPIO_STATUS_GPIO_Port, GPIO_STATUS_Pin, GPIO_PIN_RESET);
}

#include <drivers/bsp/F103xB_METEOSTATION/gpio.hpp>
#include <drivers/csp/F401xE_TEST/Core/Inc/main.h>
#include <drivers/csp/F401xE_TEST/Core/Inc/gpio.h>

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

void bsp::gpio::status::toggle()
{
    HAL_GPIO_TogglePin(GPIO_STATUS_GPIO_Port, GPIO_STATUS_Pin);
}

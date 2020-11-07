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

void bsp::gpio::status::toggle()
{
    HAL_GPIO_TogglePin(GPIO_STATUS_GPIO_Port, GPIO_STATUS_Pin);
}

bool bsp::gpio::input_button::state()
{
    return HAL_GPIO_ReadPin(GPIO_BUTTON_GPIO_Port, GPIO_BUTTON_Pin) == GPIO_PIN_SET;
}

void bsp::gpio::red::on()
{
    HAL_GPIO_WritePin(GPIO_RED_GPIO_Port, GPIO_RED_Pin, GPIO_PIN_SET);
}

void bsp::gpio::red::off()
{
    HAL_GPIO_WritePin(GPIO_RED_GPIO_Port, GPIO_RED_Pin, GPIO_PIN_RESET);
}

void bsp::gpio::red::toggle()
{
    HAL_GPIO_TogglePin(GPIO_RED_GPIO_Port, GPIO_RED_Pin);
}

void bsp::gpio::green::on()
{
    HAL_GPIO_WritePin(GPIO_GREEN_GPIO_Port, GPIO_GREEN_Pin, GPIO_PIN_SET);
}

void bsp::gpio::green::off()
{
    HAL_GPIO_WritePin(GPIO_GREEN_GPIO_Port, GPIO_GREEN_Pin, GPIO_PIN_RESET);
}

void bsp::gpio::green::toggle()
{
    HAL_GPIO_TogglePin(GPIO_GREEN_GPIO_Port, GPIO_GREEN_Pin);
}

void bsp::gpio::blue::on()
{
    HAL_GPIO_WritePin(GPIO_BLUE_GPIO_Port, GPIO_BLUE_Pin, GPIO_PIN_SET);
}

void bsp::gpio::blue::off()
{
    HAL_GPIO_WritePin(GPIO_BLUE_GPIO_Port, GPIO_BLUE_Pin, GPIO_PIN_RESET);
}

void bsp::gpio::blue::toggle()
{
    HAL_GPIO_TogglePin(GPIO_BLUE_GPIO_Port, GPIO_BLUE_Pin);
}

#include <gpio.hpp>
#include <main.h>
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

void bsp::gpio::gsm::on()
{
    HAL_GPIO_WritePin(GSM_RESET_GPIO_Port, GSM_RESET_Pin, GPIO_PIN_SET);
}

void bsp::gpio::gsm::off()
{
    HAL_GPIO_WritePin(GSM_RESET_GPIO_Port, GSM_RESET_Pin, GPIO_PIN_RESET);
}

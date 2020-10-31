#include <drivers/bsp/F103xB/usart.hpp>
#include <usart.h>

void bsp::usart::init()
{
    MX_USART1_UART_Init(); 
}

void bsp::usart::send(const uint8_t data[], std::size_t size)
{
    HAL_UART_Transmit(&huart1, const_cast<uint8_t*>(data), size, timeout);
}

bool bsp::usart::receive(uint8_t data[], std::size_t size)
{
    auto result = HAL_UART_Receive(&huart1, data, size, timeout);

    return result == HAL_OK;
}

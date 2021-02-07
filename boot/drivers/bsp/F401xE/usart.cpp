#include <usart.hpp>
#include <usart.h>

namespace
{
static bool is_started = false;
static bool _is_idle = false;
}

void bsp::usart::init()
{
    MX_USART2_UART_Init(); 
}

void bsp::usart::send(const uint8_t data[], std::size_t size)
{
    HAL_UART_Transmit(&huart2, const_cast<uint8_t*>(data), size, timeout);
}

bool bsp::usart::start_receive(uint8_t data[], std::size_t size)
{
    auto result = HAL_UART_Receive_IT(&huart2, data, size);

    if (result == HAL_OK) {
        is_started = true;
        return true;
    }

    return false;
}

bool bsp::usart::restart_receive(uint8_t data[], std::size_t size)
{
    bool result_stop = stop_receive();
    bool result_start = start_receive(data, size);

    _is_idle = false;

    return  result_stop and result_start;
}

bool bsp::usart::stop_receive()
{
    auto result = HAL_UART_AbortReceive_IT(&huart2);

    if (result == HAL_OK) {
        is_started = false;
        return true;
    }

    return false;
}

std::size_t bsp::usart::count_receive()
{
    return huart2.RxXferSize - huart2.RxXferCount;
}

bool bsp::usart::is_idle()
{
    return _is_idle;
}

extern "C" {
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (is_started)
        bsp::usart::restart_receive(huart->pRxBuffPtr, huart->RxXferSize);
}

void HAL_UART_IdleCallback(UART_HandleTypeDef *huart)
{
    _is_idle = true;
    /*
    if (is_started)
        bsp::usart::restart_receive(huart->pRxBuffPtr, huart->RxXferSize);
    */
}
}


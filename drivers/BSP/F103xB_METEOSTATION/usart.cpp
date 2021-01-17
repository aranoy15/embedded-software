#include <drivers/bsp/F103xB_METEOSTATION/usart.hpp>
#include <usart.h>
#include <drivers/lib/uart/uart.hpp>

namespace 
{

using usart_handle_t = UART_HandleTypeDef;

usart_handle_t* get_huart(bsp::usart::port_t port)
{
    using namespace bsp::usart;

    switch(port) {
        case port_t::_1:
            return &huart1;
        case port_t::_2:
            return &huart2;
        default:
            return nullptr;
    }
}

void receive_complete_callback(bsp::usart::port_t port)
{
    using namespace bsp::usart;
    using namespace lib::uart;

    if (port == port_t::_1) Uart<port_t::_1>::receive_complete();
    else if (port == port_t::_2) Uart<port_t::_2>::receive_complete();
}

void idle_callback(bsp::usart::port_t port)
{
    using namespace bsp::usart;
    using namespace lib::uart;

    if (port == port_t::_1) Uart<port_t::_1>::idle();
    else if (port == port_t::_2) Uart<port_t::_2>::idle();
}
}

void bsp::usart::init(port_t port)
{
    if (port == port_t::_1) {
        MX_USART1_UART_Init();
    } else if (port == port_t::_2) {
        MX_USART2_UART_Init();
    }
}

void bsp::usart::send(port_t port, const uint8_t data[], std::size_t size)
{
    usart_handle_t* huart = get_huart(port);

    if (huart == nullptr) return;

    HAL_UART_Transmit(huart, const_cast<uint8_t*>(data), size, size * 10);
}

bool bsp::usart::start_receive(port_t port, uint8_t data[], std::size_t size)
{
    usart_handle_t* huart = get_huart(port);

    if (huart == nullptr) return false;

    auto result = HAL_UART_Receive_IT(huart, data, size);

    return result == HAL_OK;
}

bool bsp::usart::stop_receive(port_t port) noexcept
{
    usart_handle_t* huart = get_huart(port);

    if (huart == nullptr) return false;

    auto result = HAL_UART_AbortReceive_IT(huart);

    return result == HAL_OK;
}

bool bsp::usart::restart_receive(port_t port, uint8_t data[], std::size_t size)
{
    bool res_stop = stop_receive(port);
    bool res_start = start_receive(port, data, size);

    return res_stop and res_start;
}

std::size_t bsp::usart::count_receive(port_t port)
{
    std::size_t result = 0;

    usart_handle_t* huart = get_huart(port);

    if (huart == nullptr) return result;

    result = huart->RxXferSize - huart->RxXferCount;

    return result;
}

extern "C" {
    void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
    {
        bsp::usart::port_t port = bsp::usart::port_t::_1;

        if (huart->Instance == USART1) {
            port = bsp::usart::port_t::_1;
        } else if (huart->Instance == USART2) {
            port = bsp::usart::port_t::_2;
        }

        receive_complete_callback(port);
    }

    void HAL_UART_IdleCallback(UART_HandleTypeDef *huart)
    {
        bsp::usart::port_t port = bsp::usart::port_t::_1;

        if (huart->Instance == USART1) {
            port = bsp::usart::port_t::_1;
        } else if (huart->Instance == USART2) {
            port = bsp::usart::port_t::_2;
        }

        idle_callback(port);
    }
}

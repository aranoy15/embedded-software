#include <usart.h>
#include <drivers/bsp/F401xE_TEST/usart.hpp>
#include <drivers/lib/uart/uart.hpp>
#include <drivers/csp/F401xE_TEST/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/cmsis_os2.h>

namespace 
{

using usart_handle_t = UART_HandleTypeDef;

osEventFlagsId_t transmit_events_id_1 = NULL;
osEventFlagsId_t transmit_events_id_2 = NULL;

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

void transmit_complete_callback(bsp::usart::port_t port)
{
    using namespace bsp::usart;
    using namespace lib::uart;

    if (port == port_t::_1) Uart<port_t::_1>::transmit_complete();
    else if (port == port_t::_2) Uart<port_t::_2>::transmit_complete();
}

void idle_callback(bsp::usart::port_t port)
{
    using namespace bsp::usart;
    using namespace lib::uart;

    auto huart = get_huart(port);

    if (huart == nullptr) return;

    if (port == port_t::_1) { 
        Uart<port_t::_1>::idle();
    } else if (port == port_t::_2) { 
        Uart<port_t::_2>::idle();
    }
}

}

void bsp::usart::init(port_t port)
{
    if (port == port_t::_1) {
        MX_USART1_UART_Init();

        if (transmit_events_id_1 == NULL) {
            transmit_events_id_1 = osEventFlagsNew(NULL);
        }
    } else if (port == port_t::_2) {
        MX_USART2_UART_Init();

        if (transmit_events_id_2 == NULL) {
            transmit_events_id_2 = osEventFlagsNew(NULL);
        }
    }

}

void bsp::usart::send(port_t port, const uint8_t data[], std::size_t size)
{
    usart_handle_t* huart = get_huart(port);

    if (huart == nullptr) return;

    HAL_UART_Transmit_IT(huart, const_cast<uint8_t*>(data), size);

    if (port == port_t::_1) {
        osEventFlagsWait(transmit_events_id_1, 1U, osFlagsWaitAny, osWaitForever);
    } else if (port == port_t::_2) {
        osEventFlagsWait(transmit_events_id_2, 1U, osFlagsWaitAny, osWaitForever);
    }
}

bool bsp::usart::start_receive(port_t port, uint8_t data[], std::size_t size)
{
    usart_handle_t* huart = get_huart(port);

    if (huart == nullptr) return false;

    if (port == port_t::_1) {
        HAL_UART_Receive_IT(huart, data, size);
    } else {
        HAL_UART_Receive_IT(huart, data, size);
    }

    return true;
}

bool bsp::usart::stop_receive(port_t port)
{
    usart_handle_t* huart = get_huart(port);

    if (huart == nullptr) return false;

    if (port == port_t::_1) {
        HAL_UART_Abort_IT(huart);
    } else {
        HAL_UART_Abort_IT(huart);
    }

    return true;
}

bool bsp::usart::restart_receive(port_t port, uint8_t data[], std::size_t size)
{
    stop_receive(port);
    start_receive(port, data, size);

    return true;
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

    void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
    {
        bsp::usart::port_t port = bsp::usart::port_t::_1;

        if (huart->Instance == USART1) {
            port = bsp::usart::port_t::_1;
            osEventFlagsSet(transmit_events_id_1, 1);
        } else if (huart->Instance == USART2) {
            port = bsp::usart::port_t::_2;
            osEventFlagsSet(transmit_events_id_1, 1);
        }

        transmit_complete_callback(port);
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

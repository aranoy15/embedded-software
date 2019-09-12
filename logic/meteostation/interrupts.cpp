#include <uart.h>
#include <bsp.h>

extern "C" {
void USART1_IRQHandler(void)
{
  Uart<bsp::UartPort::uartP1>::instance()->IRQHandler();
  NVIC_ClearPendingIRQ(USART1_IRQn);
}

void USART2_IRQHandler(void)
{
  Uart<bsp::UartPort::uartP2>::instance()->IRQHandler();
  NVIC_ClearPendingIRQ(USART2_IRQn);
}
}
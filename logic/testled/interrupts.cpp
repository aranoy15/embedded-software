#include <uart.h>
#include <stm32f1xx_hal.h>

extern "C" {
void USART1_IRQHandler(void)
{
  Uart<uart::UartPort::usart1>::instance()->IRQHandler();
  NVIC_ClearPendingIRQ(USART1_IRQn);
}

void USART2_IRQHandler(void)
{
  Uart<uart::UartPort::usart2>::instance()->IRQHandler();
  NVIC_ClearPendingIRQ(USART2_IRQn);
}
}
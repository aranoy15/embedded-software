#include <uart.h>
#include <bsp.h>

extern "C" {
void USART1_IRQHandler(void)
{
  Uart<bsp::uartP1>::instance()->IRQHandler();
  NVIC_ClearPendingIRQ(USART1_IRQn);
}

}
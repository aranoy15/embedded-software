#include <uart.h>

extern "C" {
void USART1_IRQHandler(void)
{
	uart::Uart<uart::usart1>::instance()->IRQHandler();
}
}
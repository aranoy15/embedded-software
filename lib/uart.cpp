#include <uart.h>

void uart::usartInitGpio(uart::UartPort port)
{
	switch (port) {
		case UartPort::usart1:
			if (__HAL_RCC_USART1_IS_CLK_DISABLED())
				__HAL_RCC_USART1_CLK_ENABLE();

			uart::usart1RxPin::setup();
			uart::usart1TxPin::setup();

			HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
			HAL_NVIC_EnableIRQ(USART1_IRQn);

			break;

		case UartPort::usart2:
			if (__HAL_RCC_USART2_IS_CLK_DISABLED())
				__HAL_RCC_USART2_CLK_ENABLE();

			uart::usart2RxPin::setup();
			uart::usart2TxPin::setup();

			HAL_NVIC_SetPriority(USART2_IRQn, 5, 0);
			HAL_NVIC_EnableIRQ(USART2_IRQn);

			break;
	}
}

USART_TypeDef* uart::port2CSP(UartPort port)
{
	switch (port) {
		case uart::UartPort::usart1:
			return USART1;
		case uart::UartPort::usart2:
			return USART2;
		case uart::UartPort::usart3:
			return USART3;
	}

	return nullptr;
}
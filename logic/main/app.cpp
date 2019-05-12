#include <stdint.h>
#include <stm32f1xx_hal.h>
#include <rcc.h>
#include <gpio.h>
#include <watchdog.h>


#include <singleton.h>
#include <time.h>
#include <uart.h>
#include <uartbuffer.h>
#include <string.h>

#include <vector>

using namespace gpio;
using namespace uart;

/*
void* operator new(size_t size)
{
	void* block = malloc(size);
	return block;
}

void operator delete(void* block)
{
	free(block);
}
*/

__attribute__((constructor))
void InitAll()
{
	HAL_Init();
	SystemInit();
	rcc::Init();
}

using errorNormal = GPIO<PinDef<CSP_GPIO_PORT_NBR_C, GPIO_PIN_13>, mOutputPP>;

int main()
{
	errorNormal::setup();
	using cuart = Uart<UartPort::usart1>;
	cuart::instance()->init(64, 115200);

	bool resultGetMessage = false;
	std::string message = "";

	for (;;) {
		errorNormal::on();
		Time::sleep(Time(500));
		errorNormal::off();
		Time::sleep(Time(500));

		std::tie(resultGetMessage, message) = cuart::instance()->read();
		if (resultGetMessage) {
			cuart::instance()->send("received message: " + message);
		}
	}
}

extern "C" {
void SysTick_Handler(void)
{
	HAL_IncTick();
}

void USART1_IRQHandler(void)
{
  HAL_UART_IRQHandler(&Uart<UartPort::usart1>::instance()->m_huart);
}
}
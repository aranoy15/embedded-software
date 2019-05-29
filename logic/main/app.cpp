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
#include <i2c.h>
#include <lcd.h>

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
using btnInput = GPIO<PinDef<CSP_GPIO_PORT_NBR_B, GPIO_PIN_10>, mInput>;
//I2C_HandleTypeDef hi2c1;

using cuart = Uart<UartPort::usart1>;
int main()
{
	errorNormal::setup();
	btnInput::setup();
	cuart::instance()->init(64, 115200);
	I2C<i2c::i2cPort1>::instance()->init();

	bool resultGetMessage = false;
	std::string message = "";


	Lcd lcd(0x27, 20, 4, 1);


	/*
	HAL_StatusTypeDef res;
	for (uint16_t i = 0; i < 128; i++) {
		res = HAL_I2C_IsDeviceReady(i2cIns.getHandle(), i << 1, 1, 10);

		if (res == HAL_OK) {
			char msg[10];
			sprintf(msg, "0x%02X", i);
			cuart::instance()->send(std::string("Find address: ") + msg);
		}
	}
	*/

	lcd.sendString("Hello!!!");
	lcd.setCursor(0, 2);
	lcd.sendString("I'm stm32f103!!!");


	/*
	char str[100];
	LCD_ini();
	sprintf(str,"Hello!!!");
	LCD_String(str);
	LCD_SetPos(0, 2);
	sprintf(str,"I'm stm32f103!");
	LCD_String(str);
	*/

	for (;;) {
		/*
		errorNormal::on();
		Time::sleep(Time(500));
		errorNormal::off();
		Time::sleep(Time(500));
		*/

		lcd.setCursor(0, 1);
		lcd.sendString("State: " + std::to_string(btnInput::state()));

		std::tie(resultGetMessage, message) = cuart::instance()->read();
		if (resultGetMessage) {
			cuart::instance()->send("received message: " + message);
		} else {
			cuart::instance()->send("no message");
		}

		Time::sleep(Time(10));
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
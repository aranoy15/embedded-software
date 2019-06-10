#include <string.h>
#include <stm32f1xx_hal.h>
#include <bootuart.h>

using namespace gpio;

BootUart::BootUart() : m_huart()
{
}

void BootUart::init(uint32_t baudrate)
{
	using usart1TxPin = GPIO<PinDef<CSP_GPIO_PORT_NBR_A, GPIO_PIN_9>, mAfPP, sHi, pUp>;
	using usart1RxPin = GPIO<PinDef<CSP_GPIO_PORT_NBR_A, GPIO_PIN_10>, mInput, sHi, pUp>; 

	usart1RxPin::setup();
	usart1TxPin::setup();

    m_huart.Instance = USART1;
    m_huart.Init.BaudRate = baudrate;
    m_huart.Init.WordLength = UART_WORDLENGTH_8B;
	m_huart.Init.StopBits = UART_STOPBITS_1;
	m_huart.Init.Parity = UART_PARITY_NONE;
	m_huart.Init.Mode = UART_MODE_TX_RX;
	m_huart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	m_huart.Init.OverSampling = UART_OVERSAMPLING_16;

    HAL_UART_Init(&m_huart);
}

void BootUart::putChar(char ch)
{
    putBuf((uint8_t*)&ch, 1);
}

void BootUart::puts(const char *str)
{
    putBuf((uint8_t*)str, strlen(str));
}

void BootUart::puts_(const char *str)
{
    puts(str);
    puts("\r\n");
}

uint8_t BootUart::getChar()
{
    uint8_t tmp = 0;
    HAL_UART_Receive(&m_huart, &tmp, 1, 1000);
    return tmp;
}

uint32_t BootUart::getLine(char *_data, uint32_t length)
{
    uint8_t* data = (uint8_t*)_data;
	uint32_t count = 0;

	while (count < length) {
		char ch = getChar();
		if (ch == '\n' or ch == 0)
			break;

		*(data++) = ch;
		count++;
	}

	return count;
}

void BootUart::putBuf(const uint8_t *buf, uint32_t length)
{
    HAL_UART_Transmit(&m_huart, (uint8_t*)buf, length, 1000);
}
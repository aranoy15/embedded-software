#include <string.h>
#include <bsp.h>

#include <bootuart.h>

BootUart::BootUart() : m_huart()
{
}

void BootUart::init()
{
	bsp::uart::usartInitGpio(bsp::log::logPort);

    m_huart.Instance = bsp::uart::port2CSP(bsp::log::logPort);
    m_huart.Init.BaudRate = 115200;
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

bool BootUart::getBuf(uint8_t* data, uint32_t length)
{
	auto result = HAL_UART_Receive(&m_huart, data, length, 1000);

	return (result == HAL_OK);
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
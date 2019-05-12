#pragma once

#include <singleton.h>
#include <stm32f1xx_hal.h>
#include <uartbuffer.h>
#include <gpio.h>
#include <tuple>
#include <memory>

namespace uart
{

using namespace gpio;

enum class UartPort
{
	usart1 = 1,
	usart2 = 2,
	usart3 = 3
};

using usart1TxPin = GPIO<PinDef<CSP_GPIO_PORT_NBR_A, GPIO_PIN_9>, mAfPP, sHi, pUp>;
using usart1RxPin = GPIO<PinDef<CSP_GPIO_PORT_NBR_A, GPIO_PIN_10>, mInput, sHi, pUp>;
}

template <uart::UartPort port>
class Uart : public Singleton<Uart<port> >
{
public:
	enum Parity
	{
		none = UART_PARITY_NONE,
		even = UART_PARITY_EVEN,
		odd = UART_PARITY_ODD
	};

	enum DataBits
	{
		db8 = UART_WORDLENGTH_8B,
		db9 = UART_WORDLENGTH_9B
	};

	enum StopBits
	{
		sb1 = UART_STOPBITS_1,
		sb2 = UART_STOPBITS_2
	};

public:
	void IRQHandler() { 
		HAL_UART_IRQHandler(&m_huart); 
	}

private:

    Uart(const Uart&);
	Uart operator=(const Uart&);

public:
	UART_HandleTypeDef m_huart;
    std::unique_ptr<UartBuffer> m_buffer;

	Uart()
	    : m_huart(),
	      m_buffer()
	{
	}

	void init(uint16_t size, uint32_t baudRate, Parity parity = none,
	          StopBits stopBits = sb1, DataBits bits = db8)
	{
		m_buffer = std::make_unique<UartBuffer>(size);

		m_huart.Instance = port2CSP();
		m_huart.Init.BaudRate = baudRate;
		m_huart.Init.WordLength = bits;
		m_huart.Init.StopBits = stopBits;
		m_huart.Init.Parity = parity;
		m_huart.Init.Mode = UART_MODE_TX_RX;
		m_huart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
		m_huart.Init.OverSampling = UART_OVERSAMPLING_16;

		usartInitGpio();
		HAL_UART_Init(&m_huart);

		HAL_UART_Receive_IT(&m_huart, m_buffer->buffer(), m_buffer->size());
	}

	void send(std::string message, char end = '\n')
	{
		message += end;

		HAL_UART_Transmit(&m_huart, (uint8_t*)message.c_str(), message.length(), 1000);
	}

	std::tuple<bool, std::string> read(char end = '\n')
	{
		std::string message = "";

		bool result = m_buffer->getFirstPacket(&message, end);

		return std::make_tuple(result, message);
	}

	static USART_TypeDef* port2CSP()
	{
		switch (port) {
			case uart::UartPort::usart1:
				return USART1;
			case uart::UartPort::usart2:
				return USART2;
			case uart::UartPort::usart3:
				return USART3;
		}
	}

private:
	static void usartInitGpio()
	{
		switch (port) {
			case uart::UartPort::usart1:
				if (__HAL_RCC_USART1_IS_CLK_DISABLED()) __HAL_RCC_USART1_CLK_ENABLE();

				uart::usart1RxPin::setup();
				uart::usart1TxPin::setup();

				HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
				HAL_NVIC_EnableIRQ(USART1_IRQn);
				break;
		}
	}
};
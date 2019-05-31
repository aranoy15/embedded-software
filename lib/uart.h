#pragma once

#include <singleton.h>
#include <stm32f1xx_hal.h>
#include <cyclicbuffer.h>
#include <gpio.h>
#include <tuple>
#include <memory>
#include <timer.h>
#include <mutex.h>
#include <cstring>

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
		uint32_t isrflags = READ_REG(m_huart.Instance->SR);
		uint32_t cr1its = READ_REG(m_huart.Instance->CR1);
		uint32_t cr3its = READ_REG(m_huart.Instance->CR3);
		uint32_t errorflags = 0x00U;
		uint32_t dmarequest = 0x00U;

		errorflags = (isrflags & (uint32_t)(USART_SR_PE | USART_SR_FE |
		                                    USART_SR_ORE | USART_SR_NE));
		if (errorflags == RESET) {
			/* UART in mode Receiver
			 * -------------------------------------------------*/
			if (((isrflags & USART_SR_RXNE) != RESET) &&
			    ((cr1its & USART_CR1_RXNEIE) != RESET)) {
				uint8_t data;
				if (m_huart.Init.Parity == UART_PARITY_NONE) {
					data = (uint8_t)(m_huart.Instance->DR & (uint8_t)0x00FF);
				} else {
					data = (uint8_t)(m_huart.Instance->DR & (uint8_t)0x007F);
				}
				m_buf.put(data);
				return;
			}
		}

		if ((errorflags != RESET) &&
		    (((cr3its & USART_CR3_EIE) != RESET) ||
		     ((cr1its & (USART_CR1_RXNEIE | USART_CR1_PEIE)) != RESET))) {
			/* UART parity error interrupt occurred
			 * ----------------------------------*/
			if (((isrflags & USART_SR_PE) != RESET) &&
			    ((cr1its & USART_CR1_PEIE) != RESET)) {
				m_huart.ErrorCode |= HAL_UART_ERROR_PE;
			}

			/* UART noise error interrupt occurred
			 * -----------------------------------*/
			if (((isrflags & USART_SR_NE) != RESET) &&
			    ((cr3its & USART_CR3_EIE) != RESET)) {
				m_huart.ErrorCode |= HAL_UART_ERROR_NE;
			}

			/* UART frame error interrupt occurred
			 * -----------------------------------*/
			if (((isrflags & USART_SR_FE) != RESET) &&
			    ((cr3its & USART_CR3_EIE) != RESET)) {
				m_huart.ErrorCode |= HAL_UART_ERROR_FE;
			}

			/* UART Over-Run interrupt occurred
			 * --------------------------------------*/
			if (((isrflags & USART_SR_ORE) != RESET) &&
			    ((cr3its & USART_CR3_EIE) != RESET)) {
				m_huart.ErrorCode |= HAL_UART_ERROR_ORE;
			}

			/* Call UART Error Call back function if need be
			 * --------------------------*/
			if (m_huart.ErrorCode != HAL_UART_ERROR_NONE) {
				/* UART in mode Receiver
				 * -----------------------------------------------*/
				if (((isrflags & USART_SR_RXNE) != RESET) &&
				    ((cr1its & USART_CR1_RXNEIE) != RESET)) {
					uint8_t data;
					if (m_huart.Init.Parity == UART_PARITY_NONE) {
						data = (uint8_t)(m_huart.Instance->DR & (uint8_t)0x00FF);
					} else {
						data = (uint8_t)(m_huart.Instance->DR & (uint8_t)0x007F);
					}
					m_buf.put(data);
				}
			}

			m_huart.ErrorCode = HAL_UART_ERROR_NONE;
		}
	}

private:

    Uart(const Uart&);
	Uart operator=(const Uart&);

	UART_HandleTypeDef m_huart;
	CyclicBuffer<uint8_t> m_buf;
	Mutex writeMutex;
	Mutex readMutex;
	Timer m_timer;

public:
    //std::unique_ptr<UartBuffer> m_buffer;

	Uart()
	    : m_huart(),
		  m_buf(),
		  writeMutex(),
		  readMutex(),
		  m_timer()
	{
	}

	void init(uint16_t size, uint32_t baudRate, Parity parity = none,
	          StopBits stopBits = sb1, DataBits bits = db8)
	{
		//m_buffer = std::make_unique<UartBuffer>(size);
		m_buf.alloc(size);

		m_huart.Instance = port2CSP();
		m_huart.Init.BaudRate = baudRate;
		m_huart.Init.WordLength = bits;
		m_huart.Init.StopBits = stopBits;
		m_huart.Init.Parity = parity;
		m_huart.Init.Mode = UART_MODE_TX_RX;
		m_huart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
		m_huart.Init.OverSampling = UART_OVERSAMPLING_16;

		usartInitGpio();

		__HAL_UART_ENABLE_IT(&m_huart, UART_IT_RXNE);
		HAL_UART_Init(&m_huart);
	}

	void send(std::string message, char end = '\n')
	{
		Lock lock(writeMutex);
		message += end;
		HAL_UART_Transmit(&m_huart, (uint8_t*)message.c_str(), message.length(), 1000);
	}

	std::string readln(uint32_t timeout = 2000)
	{
		Lock lock(readMutex);
		std::string message;

		m_timer.start(timeout);
		bool resultRead = false;

		while (not m_timer.elapsed()) {
			if (not m_buf.isEmpty()) {
				uint8_t data = m_buf.get();

				if (data == '\n') {
					resultRead = true;
					break;
				}

				message.push_back(data);
			}

			Time::sleep(Time(10));
		}

		if (not resultRead)
			message.clear();

		return message;
	}

	bool readCount(uint8_t* data, uint32_t size, uint32_t timeout = 2000)
	{
		Lock lock(readMutex);
		m_timer.start(timeout);

		while (not m_timer.elapsed()) {
			if (m_buf.getUsedSize() == size) {
				memcpy(data, m_buf.buffer(), size);
				m_buf.clear();
				return true;
			}

			Time::sleep(Time(100));
		}
		
		return false;
	}


private:

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
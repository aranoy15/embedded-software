#pragma once

#include <singleton.h>
#include <cyclicbuffer.h>
#include <tuple>
#include <memory>
#include <timer.h>
#include <cstring>
#include <utils.h>
#include <bsp.h>
#include <mutex.h>

template <bsp::UartPort port>
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
#if defined F1
	void IRQHandler()
	{
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
						data =
						    (uint8_t)(m_huart.Instance->DR & (uint8_t)0x00FF);
					} else {
						data =
						    (uint8_t)(m_huart.Instance->DR & (uint8_t)0x007F);
					}
					m_buf.put(data);
				}
			}

			m_huart.ErrorCode = HAL_UART_ERROR_NONE;
		}
	}
#endif

#if defined F4
	void IRQHandler() {
		// HAL_UART_IRQHandler(&m_huart);
		uint32_t isrflags   = READ_REG(m_huart.Instance->SR);
   		uint32_t cr1its     = READ_REG(m_huart.Instance->CR1);
   		uint32_t cr3its     = READ_REG(m_huart.Instance->CR3);
   		uint32_t errorflags = 0x00U;
   		//uint32_t dmarequest = 0x00U;

		/* If no error occurs */
		errorflags = (isrflags & (uint32_t)(USART_SR_PE | USART_SR_FE |
		                                    USART_SR_ORE | USART_SR_NE));
		if (errorflags == RESET) {
			/* UART in mode Receiver
			 * -------------------------------------------------*/
			if (((isrflags & USART_SR_RXNE) != RESET) &&
			    ((cr1its & USART_CR1_RXNEIE) != RESET)) {
				uint8_t data;
				if (m_huart.Init.Parity == UART_PARITY_NONE) {
					data =
					    (uint8_t)(m_huart.Instance->DR & (uint8_t)0x00FF);
				} else {
					data =
					    (uint8_t)(m_huart.Instance->DR & (uint8_t)0x007F);
				}

				m_buf.put(data);
				return;
			}
		}

		/* If some errors occur */
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

			/* UART in mode Receiver
			 * -----------------------------------------------*/
			if (((isrflags & USART_SR_RXNE) != RESET) &&
			    ((cr1its & USART_CR1_RXNEIE) != RESET)) {
				uint8_t data;
				if (m_huart.Init.Parity == UART_PARITY_NONE) {
					data =
					    (uint8_t)(m_huart.Instance->DR & (uint8_t)0x00FF);
				} else {
					data =
					    (uint8_t)(m_huart.Instance->DR & (uint8_t)0x007F);
				}

				m_buf.put(data);
			}

			m_huart.ErrorCode = HAL_UART_ERROR_NONE;
		}
	}
#endif

private:

    Uart(const Uart&);
	Uart operator=(const Uart&);

	UART_HandleTypeDef m_huart;
	CyclicBuffer<uint8_t> m_buf;
	//Timer m_timer;

	Mutex mMutex;

public:

	Uart()
	    : m_huart(),
		  m_buf(),
		  //m_timer(),
		  mMutex()
	{
	}

	void init(uint16_t size, uint32_t baudRate, Parity parity = none,
	          StopBits stopBits = sb1, DataBits bits = db8)
	{
		Lock lock(mMutex);

		m_buf.alloc(size);

		m_huart.Instance = bsp::uart::port2CSP(port);
		m_huart.Init.BaudRate = baudRate;
		m_huart.Init.WordLength = bits;
		m_huart.Init.StopBits = stopBits;
		m_huart.Init.Parity = parity;
		m_huart.Init.Mode = UART_MODE_TX_RX;
		m_huart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
		m_huart.Init.OverSampling = UART_OVERSAMPLING_16;

		//usartInitGpio();
		bsp::uart::usartInitGpio(port);

		__HAL_UART_ENABLE_IT(&m_huart, UART_IT_RXNE);
		HAL_UART_Init(&m_huart);
	}

	void send(std::string message, char end = '\n')
	{
		Lock lock(mMutex);
		message += end;
		HAL_UART_Transmit(&m_huart, (uint8_t*)message.c_str(), message.length(), 1000);
	}

	void dump(uint8_t *data, uint16_t size, std::string prefix = "")
	{
		std::string result;

		result.append(prefix);
		for (uint8_t i = 0; i < size; i++) {
			char hex[10];
			sprintf(hex, "0x%02X ", data[i]);
			result.append(hex);
		}

		send(result);
	}

	void send(uint8_t data[], uint16_t size)
	{
		Lock lock(mMutex);
		HAL_UART_Transmit(&m_huart, data, size, 1000);
	}

	std::string readln(uint32_t timeout = 2000)
	{
		Lock lock(mMutex);

		std::string message;

		Timer timer;
		timer.start(timeout);

		bool resultRead = false;

		if (not m_buf.isEmpty()) {
			while (m_buf.getUsedSize() != 0 and not timer.elapsed()) {
				uint8_t data = m_buf.get();

				if (data == '\n') {
					resultRead = true;
					break;
				}

				message.push_back(data);
			}
		}

		if (not resultRead)
			message.clear();

		return message;
	}

	bool readCount(uint8_t* data, uint32_t size, uint32_t timeout = 2000)
	{
		Lock lock(mMutex);

		Timer timer;
		timer.start(timeout);

		while (not timer.elapsed()) {
			if (m_buf.getUsedSize() == size) {
				memcpy(data, m_buf.buffer(), size);
				m_buf.clear();
				return true;
			}

			Time::sleep(Time(10));
		}
		
		return false;
	}

};
#pragma once

#include <singleton.h>
#include <stm32f1xx_hal.h>
#include <gpio.h>

namespace i2c
{
using namespace gpio;

enum I2CPorts
{
	i2cPort1 = 1,
	i2cPort2 = 2
};

using i2cPins =
    GPIO<PinDef<CSP_GPIO_PORT_NBR_B, GPIO_PIN_6 | GPIO_PIN_7>, mAfOD, sHi, pUp>;
}  // namespace i2c

template<i2c::I2CPorts port>
class I2C : public Singleton<I2C<port> >
{
public:
	I2C_HandleTypeDef m_hndl;
	const uint32_t timeout = 100;

private:
	I2C(const I2C&);
	I2C operator=(const I2C&);

public:
	I2C() : m_hndl()
	{
	}

	I2C_HandleTypeDef* getHandle() { return &m_hndl; }

	void init()
	{
		m_hndl.Instance = port2CSP();
		m_hndl.Init.ClockSpeed = 100000;
		m_hndl.Init.DutyCycle = I2C_DUTYCYCLE_2;
		m_hndl.Init.OwnAddress1 = 0;
		m_hndl.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
		m_hndl.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
		m_hndl.Init.OwnAddress2 = 0;
		m_hndl.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
		m_hndl.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

		initGpio();

		HAL_I2C_Init(&m_hndl);
	}

	void writeByte(uint16_t address, uint8_t data)
	{
		HAL_I2C_Master_Transmit(&m_hndl, address << 1, &data, 1, timeout);
	}

	uint8_t readByte(uint16_t address)
	{
		uint8_t result;
		HAL_I2C_Master_Receive(&m_hndl, address << 1, &result, 1, timeout);
		return result;
	}

private:
	static void initGpio()
	{
		switch(port) {
			case i2c::i2cPort1:
				if (__HAL_RCC_I2C1_IS_CLK_DISABLED()) __HAL_RCC_I2C1_CLK_ENABLE();

				i2c::i2cPins::setup();
				break;
		}
	}

	static I2C_TypeDef* port2CSP()
	{
		if (port == i2c::i2cPort1) return I2C1;

		return I2C2;
	}
};
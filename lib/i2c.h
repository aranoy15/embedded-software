#pragma once

#include <singleton.h>
#include <vector>
#include <mutex.h>
#include <string.h>
#include <bsp.h>


template<bsp::I2CPort port>
class I2C : public Singleton<I2C<port> >
{
public:
	I2C_HandleTypeDef m_hndl;
	Mutex m_mutex;
	const uint32_t timeout = 100;

private:
	I2C(const I2C&);
	I2C operator=(const I2C&);

public:
	I2C() : m_hndl(), m_mutex() {}

	I2C_HandleTypeDef* getHandle() { return &m_hndl; }

	void init()
	{
		m_hndl.Instance = bsp::i2c::port2CSP(port);
		m_hndl.Init.ClockSpeed = 100000;
		m_hndl.Init.DutyCycle = I2C_DUTYCYCLE_2;
		m_hndl.Init.OwnAddress1 = 0;
		m_hndl.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
		m_hndl.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
		m_hndl.Init.OwnAddress2 = 0;
		m_hndl.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
		m_hndl.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

		bsp::i2c::init(port);

		HAL_I2C_Init(&m_hndl);
	}

	void writeByte(uint16_t address, uint8_t data)
	{
		Lock lock(m_mutex);
		HAL_I2C_Master_Transmit(&m_hndl, address << 1, &data, 1, timeout);
	}

	bool write(uint16_t address, uint8_t data[], uint16_t size)
	{
		Lock lock(m_mutex);
		return HAL_I2C_Master_Transmit(&m_hndl, address << 1, data, size, timeout) == HAL_OK;
	}

	uint8_t readByte(uint16_t address)
	{
		uint8_t result;

		Lock lock(m_mutex);

		HAL_I2C_Master_Receive(&m_hndl, address << 1, &result, 1, timeout);
		return result;
	}

	uint8_t writeData(uint16_t address, uint8_t data[], uint32_t size)
	{
		Lock lock(m_mutex);
		return HAL_I2C_Master_Transmit(&m_hndl, address << 1, data, size, timeout);	
	}

	bool readData(uint16_t address, uint8_t data[], uint32_t sizeData, uint8_t result[], uint32_t sizeResult)
	{
		Lock lock(m_mutex);

		if (HAL_I2C_Master_Transmit(&m_hndl, address << 1, data, sizeData, timeout) != HAL_OK)
			return false;

		if (HAL_I2C_Master_Receive(&m_hndl, address << 1, result, sizeResult, timeout) != HAL_OK)
			return false;
		
		return true;
	}

	std::vector<uint8_t> searchAddresses()
	{
		std::vector<uint8_t> result;

		Lock lock(m_mutex);

		for (uint16_t i = 1; i < 128; i++) {
			bool resRead =
			    (HAL_I2C_IsDeviceReady(&m_hndl, i << 1, 1, 10) == HAL_OK);

			if (resRead) result.push_back(i);
		}

		return result;
	}

	template <typename TypeReg, typename TypeData>
	bool writeReg(uint16_t address, TypeReg reg, TypeData data)
	{
		uint8_t sendingData[sizeof(TypeReg) + sizeof(TypeData)];

		Lock lock(m_mutex);

		memcpy(sendingData, reinterpret_cast<uint8_t*>(&reg), sizeof(TypeReg));
		memcpy(sendingData + sizeof(TypeReg), reinterpret_cast<uint8_t*>(&data),
		       sizeof(TypeData));

		return HAL_I2C_Master_Transmit(&m_hndl, address << 1, sendingData, sizeof(sendingData), timeout) == HAL_OK;
	}

	template <typename TypeResult, typename TypeReg>
	TypeResult readReg(uint16_t address, TypeReg reg)
	{
		TypeResult resultData;
		readReg(address, reg, reinterpret_cast<uint8_t*>(&resultData), sizeof(TypeResult));
		return resultData;
	}

	template <typename TypeReg>
	bool readReg(uint16_t address, TypeReg reg, uint8_t* data, uint16_t size)
	{
		Lock lock(m_mutex);

		if (HAL_I2C_Master_Transmit(&m_hndl, address << 1,
									reinterpret_cast<uint8_t*>(&reg),
									sizeof(TypeReg), timeout) != HAL_OK)
			return false;

		if (HAL_I2C_Master_Receive(&m_hndl, address << 1, data, size, timeout) != HAL_OK)
			return false;

		return true;
	}
};
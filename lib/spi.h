#pragma once

#include <mutex.h>
#include <singleton.h>
#include <bsp.h>

template <bsp::SpiPort port>
class Spi;

template <bsp::SpiPort port>
class ChipSelectTrigger
{
private:
	uint8_t m_chip;

public:
    ChipSelectTrigger(uint8_t chip) : m_chip(chip)
    {
        Spi<port>::instance()->lock();
        bsp::spi::chipSelect(m_chip);
    }

    ~ChipSelectTrigger()
    {
        bsp::spi::chipUnselect(m_chip);
        Spi<port>::instance()->unlock();
    }
};

template <bsp::SpiPort port>
class Spi : public Singleton<Spi<port>>
{
private:
	friend class ChipSelectTrigger<port>;

    Spi(const Spi&);
	Spi operator=(const Spi&);

private:
	Mutex m_mutex;
	uint16_t m_timeout;
	SPI_HandleTypeDef m_config;

	void lock() { m_mutex.lock(); }
	void unlock() { m_mutex.unlock(); }

public:
    Spi() : m_mutex(), m_timeout(100), m_config() {}
	virtual ~Spi() { deinit(); }

	bool init()
    {
        m_config.Instance = bsp::spi::port2CSP(port);
        m_config.Init.Mode = SPI_MODE_MASTER;
        m_config.Init.Direction = SPI_DIRECTION_2LINES;
        m_config.Init.DataSize = SPI_DATASIZE_8BIT;
        m_config.Init.CLKPolarity = SPI_POLARITY_LOW;
        m_config.Init.CLKPhase = SPI_PHASE_1EDGE;
        m_config.Init.NSS = SPI_NSS_SOFT;
        m_config.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
        m_config.Init.FirstBit = SPI_FIRSTBIT_MSB;
        m_config.Init.TIMode = SPI_TIMODE_DISABLE;
        m_config.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
        m_config.Init.CRCPolynomial = 10;

        bsp::spi::init(port);
        return HAL_SPI_Init(&m_config) == HAL_OK;
    }

    void deinit()
    {
        HAL_SPI_DeInit(&m_config);
    }

    uint16_t getTimeout() const { return m_timeout; }

    uint8_t receiveByte()
    {
        uint8_t byte;
        receive(&byte, 1);
        return byte;
    }

    bool receive(uint8_t* bytes, uint32_t size)
    {
        Lock lock(m_mutex);
        return HAL_SPI_Receive(&m_config, bytes, size, getTimeout() * size) == HAL_OK;
	}

    bool sendByte(uint8_t byte)
    {
        return send(&byte, 1);
    }

    bool send(const uint8_t* bytes, uint32_t size)
    {
        Lock lock(m_mutex);
        return HAL_SPI_Transmit(&m_config, const_cast<uint8_t*>(bytes), size, getTimeout() * size) == HAL_OK;
    }

    bool sendRead(uint8_t* sendBytes, uint8_t* receiveBytes, uint32_t size)
    {
        Lock lock(m_mutex);

        return HAL_SPI_TransmitReceive(&m_config, sendBytes, receiveBytes, size, getTimeout() * size) == HAL_OK;
    }
};
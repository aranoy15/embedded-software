#pragma once

#include <gpio.h>
#include <relay.h>
//#include <mutex.h>
#include <singleton.h>
#include <stm32f1xx_hal.h>

namespace spi
{
    using namespace gpio;

    enum SpiPort
    {
        spiPort1 = 1,
        spiPort2 = 2
    };

    using adcSck = GPIO<PinDef<CSP_GPIO_PORT_NBR_A, GPIO_PIN_5>, mAfPP, sHi>;
    using adcMiso = GPIO<PinDef<CSP_GPIO_PORT_NBR_A, GPIO_PIN_6>, mInput>;
    using adcMosi = GPIO<PinDef<CSP_GPIO_PORT_NBR_A, GPIO_PIN_7>, mAfPP, sHi>;

    using adcCsPin = GPIO<PinDef<CSP_GPIO_PORT_NBR_A, GPIO_PIN_4>, mOutputPP>;
    using adcCs = Relay<adcCsPin, PinManageLogic::Inverse>;

    using adcBusy = GPIO<PinDef<CSP_GPIO_PORT_NBR_A, GPIO_PIN_3>, mInput>;

    void init(SpiPort port);
    void chipSelect(SpiPort port);
    void chipUnselect(SpiPort port);
}

template <spi::SpiPort port>
class Spi;

template <spi::SpiPort port>
class ChipSelectTrigger
{
private:
	uint8_t m_chip;

public:
    ChipSelectTrigger(uint8_t chip) : m_chip(chip)
    {
        //Spi<port>::instance()->lock();
        spi::chipSelect(m_chip);
    }

    ~ChipSelectTrigger()
    {
        spi::chipUnselect(m_chip);
        //Spi<port>::instance()->unlock();
    }
};

template <spi::SpiPort port>
class Spi : public Singleton<Spi<port>>
{
private:
	friend class ChipSelectTrigger<port>;

    Spi(const Spi&);
	Spi operator=(const Spi&);

private:
	//Mutex m_mutex;
	uint16_t m_timeout;
	SPI_HandleTypeDef m_config;

	//void lock() { m_mutex.lock(); }
	//void unlock() { m_mutex.unlock(); }

public:
    Spi() : /*m_mutex(),*/ m_timeout(100), m_config() {}
	virtual ~Spi() { deinit(); }

	bool init()
    {
        m_config.Instance = port2CSP();
        m_config.Init.Mode = SPI_MODE_MASTER;
        m_config.Init.Direction = SPI_DIRECTION_2LINES;
        m_config.Init.DataSize = SPI_DATASIZE_8BIT;
        m_config.Init.CLKPolarity = SPI_POLARITY_LOW;
        m_config.Init.CLKPhase = SPI_PHASE_1EDGE;
        m_config.Init.NSS = SPI_NSS_SOFT;
        m_config.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
        m_config.Init.FirstBit = SPI_FIRSTBIT_MSB;
        m_config.Init.TIMode = SPI_TIMODE_DISABLE;
        m_config.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
        m_config.Init.CRCPolynomial = 10;

        spi::init(port);
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
        return HAL_SPI_Receive(&m_config, bytes, size, getTimeout() * size) == HAL_OK;
	}

    bool sendByte(uint8_t byte)
    {
        return send(&byte, 1);
    }

    bool send(const uint8_t* bytes, uint32_t size)
    {
        return HAL_SPI_Transmit(&m_config, const_cast<uint8_t*>(bytes), size, getTimeout() * size) == HAL_OK;
    }

private:
	SPI_TypeDef* port2CSP()
	{
		switch (port) {
			case spi::spiPort1:
				return SPI1;
			case spi::spiPort2:
				return SPI2;
		}
	}
};
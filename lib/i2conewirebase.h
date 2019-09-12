#pragma once

#include <bsp.h>
#include <inttypes.h>
#include <i2c.h>

class I2COnewireBase
{
private:
	enum
	{
		DS2482_STATUS_BUSY = 1 << 0,
		DS2482_CONFIG_APU = 1 << 0,
		DS2482_STATUS_PPD = 1 << 1,
		DS2482_STATUS_SBR = 1 << 5,
		DS2482_STATUS_TSB = 1 << 6,
		DS2482_STATUS_DIR = 1 << 7
	};

    enum
    {
        PTR_STATUS = 0xF0,
        PTR_READ = 0xE1,
        PTR_CONFIG = 0xC3
    };

private:
	using I2CType =  I2C<bsp::i2conewire::i2cOnewirePort>;
	I2CType& i2c() { return *I2CType::instance(); }

	Mutex m_mutex;
    const uint16_t baseAddress = 0x18;
    uint16_t address;

private:
    bool reset();
    bool config(uint8_t);
    uint8_t busyWait(bool = false);
    uint8_t wireReadStatus(bool);
    void setReadPtr(uint8_t);

    uint8_t searchAddress[8];
    uint8_t searchLastDisrepancy;
    uint8_t searchExhausted;

protected:
    uint8_t crc8(uint8_t[], uint8_t);

    //void init();
    bool detect();
    bool channelSelect(uint8_t); 

    bool wireReset();
    uint8_t wireSearch(uint8_t*);
    void wireResetSearch();
    void wireWriteByte(uint8_t);

    uint8_t wireReadByte();
    void wireWriteBit(uint8_t);
    uint8_t wireReadBit();
    void wireSkip();
    void wireSelect(uint8_t[]);

public:
    I2COnewireBase();
    virtual ~I2COnewireBase();

    virtual std::tuple<bool, uint16_t> update(uint8_t, int = 1) = 0;
};
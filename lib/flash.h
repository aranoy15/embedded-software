#pragma once

#include <singleton.h>
#include <spi.h>
#include <bsp.h>

class Flash : public Singleton<Flash>
{
private:
	enum
	{
		readStatusCmd = 0x05,
		readCmd = 0x0B,
		writeEnableCmd = 0x06,
		writeDisableCmd = 0x04,
		writeCmd = 0x02,
		writeStatusCmd = 0x01,
		protectSectorCmd = 0x36,
		unprotectSectodCmd = 0x39,
		readSectorProtectCmd = 0x3C,
		erase4KCmd = 0x20,
		eraseChipCmd = 0xC7
	};

private:
	using flashSpi = Spi<bsp::flash::port>;
	using FlashChipSelectTrigger = ChipSelectTrigger<bsp::flash::port>;

	Mutex m_mutex;

	flashSpi& getSpi() const { return *flashSpi::instance(); }

public:
    Flash();

	void read(uint32_t addr, uint8_t* aData, uint32_t size);
	void write(uint32_t addr, const uint8_t* aData, uint32_t size);
	void writeData(uint32_t addr, const uint8_t* aData, uint32_t size);
	void writeEnable(bool enable);
	uint8_t readStatus();
	void waitForReady();
	void erase4k(uint32_t addr);
	//void fullErasing();

private:
	void sendByte(uint8_t b);
	void sendData(const uint8_t* _data, uint32_t size);
	uint8_t recvByte();
	void sendAddr(int32_t addr);
	bool getSectorProtection(uint32_t addr);
	void setSectorProtection(uint32_t addr, bool enable);
	void eraseChip();
	void setProtection(bool enable);
};
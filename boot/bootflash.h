#ifndef BOOTLOADER_FLASH_H
#define BOOTLOADER_FLASH_H

#include <csp.h>

#define BSP_EXT_FLASH_MEM_CS 	1<<16

#define READ_STATUS_CMD         0x05
#define READ_CMD                0x0B
#define WRITE_ENABLE_CMD        0x06
#define WRITE_DISABLE_CMD       0x04
#define WRITE_CMD               0x02
#define WRITE_STATUS_CMD        0x01
#define PROTECT_SECTOR_CMD      0x36
#define UNPROTECT_SECTOR_CMD    0x39
#define READ_SECTOR_PROTECT_CMD 0x3C
#define ERASE_4K_CMD            0x20
#define ERASE_CHIP_CMD          0xC7

class BootFlash
{
private:
	SPI_HandleTypeDef m_cfg;

public:
	BootFlash();

	void init();
	void chipSelect();
	void chipUnselect();

	void read(uint32_t addr, uint8_t* aData, uint32_t size);
	void write(uint32_t addr, const uint8_t* aData, uint32_t size);
	void writeEnable(bool enable);
	uint8_t readStatus();
	void waitForReady();
	void erase4k(uint32_t addr);
	void fullErasing();
	void writeData(uint32_t addr, const uint8_t* aData, uint32_t size);

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

#endif /* BOOTLOADER_FLASH_H */
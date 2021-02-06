#pragma once

#include <singleton.h>
#include <spi.h>

#include <ff.h>
#include <diskio.h>

class Sdio : public Singleton<Sdio>
{
    Sdio(const Sdio&) = delete;
    Sdio operator=(const Sdio&) = delete;

private:
    volatile uint8_t mStatus;

    using SdioSpi = Spi<bsp::sdio::port>;
	using SdioChipSelectTrigger = ChipSelectTrigger<bsp::sdio::port>;

	SdioSpi& spi() const { return *SdioSpi::instance(); }

    enum CardType
    {
        MMC = 0x01,
        SD1 = 0x02,
        SD2 = 0x04,
        SDC = SD1 | SD2,
        BLOCK = 0x08
    };

    enum Commands
    {
        CMD0 = 0x40 + 0,
        CMD1 = 0x40 + 1,
        ACMD41 = 0xC0 + 41,
        CMD8 = 0x40 + 8,
        CMD9 = 0xFE,
        CMD16 = 0x40 + 16,
        CMD17 = 0x40 + 17,
        CMD24 = 0x40 + 24,
        CMD55 = 0x40 + 55,
        CMD58 = 0x40 + 58
    };

private:
    bool waitDataToken(uint8_t token);
    uint8_t readByte();
    bool readBytes(uint8_t* buff, uint32_t size);
    bool waitNotBusy();

public:
	Sdio() : mStatus(0) { bsp::sdio::init(); }

    bool init();
    uint32_t getBlocksNumber();
    bool readSingleBlock(uint32_t blockNum, uint8_t* buff);
    bool writeSingleBlock(uint32_t blockNum, const uint8_t* buff);

    bool readBegin(uint32_t blockNum);
    bool readData(uint8_t* buff);
    bool readEnd();

    bool writeBegin(uint32_t blockNum);
    bool writeData(uint8_t* buff);
    bool writeEnd();

    DSTATUS checkStatus();
    DSTATUS initialize();
    DRESULT read(BYTE* buff, DWORD sector, UINT count);
    DRESULT write(BYTE* buff, DWORD sector, UINT count);
    DRESULT ioctl(BYTE cmd, void* buff);
};

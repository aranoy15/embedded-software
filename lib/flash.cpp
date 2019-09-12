#include <flash.h>
#include <time.h>

Flash::Flash() : m_mutex()
{
    bsp::flash::init();
}

void Flash::sendByte(uint8_t b)
{
	sendData(&b, 1);
}

void Flash::sendData(const uint8_t* data, uint32_t size)
{
    getSpi().send(data, size);
}

uint8_t Flash::recvByte()
{
    return getSpi().receiveByte();
}

void Flash::sendAddr(int32_t addr)
{
	sendByte(uint8_t(addr >> 16));
	sendByte(uint8_t(addr >> 8));
	sendByte(uint8_t(addr));
}

void Flash::read(uint32_t addr, uint8_t* data, uint32_t size)
{
    FlashChipSelectTrigger fcsTriegger(bsp::spi::Flash);
	sendByte(readCmd);
	sendAddr(addr);
	sendByte(0x00);

	for (uint32_t i = 0; i < size; i++)
		data[i] = recvByte();
}

void Flash::writeEnable(bool enable)
{
    FlashChipSelectTrigger fcsTriegger(bsp::spi::Flash);
	sendByte(enable ? writeEnableCmd : writeDisableCmd);
}

void Flash::write(uint32_t addr, const uint8_t* aData, uint32_t size)
{
	const uint8_t* data = (const uint8_t*)aData;

    {
        FlashChipSelectTrigger fcsTriegger(bsp::spi::Flash);
	    sendByte(writeCmd);
	    sendAddr(addr);

    	for (uint32_t i = 0; i < size; ++i)
    		sendByte(data[i]);
    }

    Time::sleep(Time(3));
}

void Flash::writeData(uint32_t addr, const uint8_t* aData, uint32_t size)
{
	if (size == 0) return;

	const uint8_t* data = (const uint8_t*)aData;

	for (uint16_t offset = 0; offset != size;) {
		const uint16_t bytesBeforePageEnd = 256 - ((addr + offset) % 256);
		const uint16_t unwritenSize = size - offset;
		const uint16_t blockSize =
		    (bytesBeforePageEnd > unwritenSize ? unwritenSize
		                                       : bytesBeforePageEnd);

		writeEnable(true);
		setSectorProtection(addr + offset, false);
		waitForReady();

		writeEnable(true);
		write(addr + offset, data + offset, blockSize);
		waitForReady();

		offset += blockSize;
		if (!(offset < 4 * 1024 - 256)) break;
	}
}

void Flash::erase4k(uint32_t addr)
{
	writeEnable(true);
	setSectorProtection(addr, false);
	waitForReady();
	writeEnable(true);
	{
		FlashChipSelectTrigger fcsTriegger(bsp::spi::Flash);
		sendByte(erase4KCmd);
		sendAddr(addr);
	}
	waitForReady();

    Time::sleep(Time(3));
}

void Flash::waitForReady()
{
	while (readStatus() & 1)
		;
}

uint8_t Flash::readStatus()
{
    uint8_t status = 0;
	{
		FlashChipSelectTrigger fcsTriegger(bsp::spi::Flash);
		sendByte(readStatusCmd);
		status = recvByte();
	}
	return status;
}

void Flash::setProtection(bool enable)
{
    FlashChipSelectTrigger fcsTriegger(bsp::spi::Flash);
	sendByte(writeStatusCmd);
	if (enable) {
		sendByte(0x3c);
	} else {
		sendByte(0x0);
	}
}

void Flash::setSectorProtection(uint32_t addr, bool enable)
{
    FlashChipSelectTrigger fcsTriegger(bsp::spi::Flash);
	sendByte(enable ? protectSectorCmd : unprotectSectodCmd);
	sendAddr(addr);
}

bool Flash::getSectorProtection(uint32_t addr)
{
    uint8_t status = 0;
	{
		FlashChipSelectTrigger fcsTriegger(bsp::spi::Flash);
		sendByte(readSectorProtectCmd);
		sendAddr(addr);
		status = recvByte();
	}

	return status;
}
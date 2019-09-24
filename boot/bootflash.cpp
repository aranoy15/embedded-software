#include <bsp.h>
#include <sleep.h>

#include <bootflash.h>

BootFlash::BootFlash() : m_cfg()
{
}

void BootFlash::chipSelect()
{
    bsp::flash::flashCs::on();
}

void BootFlash::chipUnselect()
{
    bsp::flash::flashCs::off();
}

void BootFlash::init()
{
    bsp::spi::init(bsp::spiP1);
    bsp::flash::init();
    sleep(50);

    m_cfg.Instance = bsp::spi::port2CSP(bsp::spiP1);
    m_cfg.Init.Mode = SPI_MODE_MASTER;
    m_cfg.Init.Direction = SPI_DIRECTION_2LINES;
    m_cfg.Init.DataSize = SPI_DATASIZE_8BIT;
    m_cfg.Init.CLKPolarity = SPI_POLARITY_LOW;
    m_cfg.Init.CLKPhase = SPI_PHASE_1EDGE;
    m_cfg.Init.NSS = SPI_NSS_SOFT;
    m_cfg.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
    m_cfg.Init.FirstBit = SPI_FIRSTBIT_MSB;
    m_cfg.Init.TIMode = SPI_TIMODE_DISABLE;
    m_cfg.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    m_cfg.Init.CRCPolynomial = 10;  

    HAL_SPI_Init(&m_cfg);
}

void BootFlash::sendByte(uint8_t b)
{
	sendData(&b, 1);
}

void BootFlash::sendData(const uint8_t* data, uint32_t size)
{
	HAL_SPI_Transmit(&m_cfg, (uint8_t*)data, size, 1000);
}

uint8_t BootFlash::recvByte()
{
	uint8_t b = 0;
	HAL_SPI_Receive(&m_cfg, &b, 1, 1000);
	return b;
}

void BootFlash::sendAddr(int32_t addr)
{
	sendByte(uint8_t(addr >> 16));
	sendByte(uint8_t(addr >> 8));
	sendByte(uint8_t(addr));
}

void BootFlash::read(uint32_t addr, uint8_t* data, uint32_t size)
{
	chipSelect();
	sendByte(READ_CMD);
	sendAddr(addr);
	sendByte(0x00);

	for (uint32_t i = 0; i < size; i++)
		data[i] = recvByte();
	chipUnselect();
}

void BootFlash::writeEnable(bool enable)
{
	chipSelect();
	sendByte(enable ? WRITE_ENABLE_CMD : WRITE_DISABLE_CMD);
	chipUnselect();
}

void BootFlash::write(uint32_t addr, const uint8_t* aData, uint32_t size)
{
	const uint8_t* data = (const uint8_t*)aData;

	chipSelect();

	sendByte(WRITE_CMD);
	sendAddr(addr);

	for (uint32_t i = 0; i < size; ++i)
		sendByte(data[i]);

	chipUnselect();

	for (int tmp = 10000; tmp > 0; tmp--)
		;
}

void BootFlash::writeData(uint32_t addr, const uint8_t* aData, uint32_t size)
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

void BootFlash::erase4k(uint32_t addr)
{
	writeEnable(true);
	setSectorProtection(addr, false);
	waitForReady();
	writeEnable(true);
	chipSelect();
	sendByte(ERASE_4K_CMD);
	sendAddr(addr);
	chipUnselect();
	waitForReady();

	for (int tmp = 10000; tmp > 0; tmp--)
		;
}

void BootFlash::setSectorProtection(uint32_t addr, bool enable)
{
	chipSelect();
	sendByte(enable ? PROTECT_SECTOR_CMD : UNPROTECT_SECTOR_CMD);
	sendAddr(addr);
	chipUnselect();
}

bool BootFlash::getSectorProtection(uint32_t addr)
{
	chipSelect();
	sendByte(READ_SECTOR_PROTECT_CMD);
	sendAddr(addr);
	uint8_t status = recvByte();
	chipUnselect();

	return status;
}

void BootFlash::waitForReady()
{
	while (readStatus() & 1)
		;
}

uint8_t BootFlash::readStatus()
{
	chipSelect();
	sendByte(READ_STATUS_CMD);
	uint8_t status = recvByte();
	chipUnselect();
	return status;
}

void BootFlash::eraseChip()
{
	chipSelect();
	sendByte(ERASE_CHIP_CMD);
	chipUnselect();
}


void BootFlash::fullErasing()
{
	waitForReady();
	writeEnable(true);
	setProtection(false);
	waitForReady();
	writeEnable(true);
	eraseChip();
	waitForReady();
}


void BootFlash::setProtection(bool enable)
{
	chipSelect();
	sendByte(WRITE_STATUS_CMD);
	if (enable) {
		sendByte(0x3c);
	} else {
		sendByte(0x0);
	}
	chipUnselect();
}
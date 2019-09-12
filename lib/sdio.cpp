#include <sdio.h>


uint8_t Sdio::readByte()
{
    uint8_t result;

    uint8_t tx = 0xFF;

    for(;;) {
        spi().sendRead(&tx, &result, sizeof(result));

        if ((result & 0x80) == 0) break;
    }

    return result;
}


bool Sdio::waitDataToken(uint8_t token)
{
    uint8_t fb;
    uint8_t tx = 0xFF;

    for (;;) {
        spi().sendRead(&tx, &fb, sizeof(fb));

        if (fb == token)
            break;
        
        if (fb != 0xFF)
            return false;
    }

    return true;
}

bool Sdio::readBytes(uint8_t* buff, uint32_t size)
{
    uint8_t tx = 0xFF;

    while (size > 0) {
        spi().sendRead(&tx, buff, 1);
        buff++;
        size--;
    }

    return true;
}

bool Sdio::waitNotBusy()
{
    uint8_t busy;

    do {
        if (not readBytes(&busy, sizeof(busy)))
            return false;
    } while (busy != 0xFF);

    return true;
}

bool Sdio::init()
{
	for (uint8_t i = 0; i < 10; i++) {
		spi().sendByte(0xFF);
	}

	SdioChipSelectTrigger trigger(bsp::spi::Sdio);

	if (not waitNotBusy()) return false;

	{
		static const uint8_t cmd[] = {
		    0x40 | 0x00 /* CMD0 */, 0x00,           0x00, 0x00,
		    0x00 /* ARG = 0 */,     (0x4A << 1) | 1 /* CRC7 + end bit */};

		spi().send(cmd, sizeof(cmd));
	}

	if (readByte() != 0x01) return false;

	if (not waitNotBusy()) return false;

	{
		static const uint8_t cmd[] = {
		    0x40 | 0x08 /* CMD8 */, 0x00,           0x00, 0x01,
		    0xAA /* ARG */,         (0x43 << 1) | 1 /* CRC7 + end bit */};
		spi().send(cmd, sizeof(cmd));
	}

	if (readByte() != 0x01) return false;

	{
		uint8_t resp[4];

		if (not readBytes(resp, sizeof(resp))) return false;
		if (((resp[2] & 0x01) != 1) || (resp[3] != 0xAA)) return false;
	}

	for (;;) {
		if (not waitNotBusy()) return false;

		{
			static const uint8_t cmd[] = {
			    0x40 | 0x37 /* CMD55 */, 0x00,           0x00, 0x00,
			    0x00 /* ARG */,          (0x7F << 1) | 1 /* CRC7 + end bit */};
			spi().send(cmd, sizeof(cmd));
		}

		
		if (readByte() != 0x01) return false;

		if (not waitNotBusy()) return false;

		{
			static const uint8_t cmd[] = {
			    0x40 | 0x29 /* ACMD41 */, 0x40,           0x00, 0x00,
			    0x00 /* ARG */,           (0x7F << 1) | 1 /* CRC7 + end bit */};
			spi().send(cmd, sizeof(cmd));
		}

		uint8_t rb = readByte();
		if (rb == 0x00) break;

		if (rb != 0x01) return false;
	}

	if (not waitNotBusy()) return false;

	{
		static const uint8_t cmd[] = {
		    0x40 | 0x3A /* CMD58 */, 0x00,           0x00, 0x00,
		    0x00 /* ARG */,          (0x7F << 1) | 1 /* CRC7 + end bit */};
		spi().send(cmd, sizeof(cmd));
	}

	if (readByte() != 0x00) return false;

	{
		uint8_t resp[4];
		if (not readBytes(resp, sizeof(resp))) return false;

		if ((resp[0] & 0xC0) != 0xC0) return false;
	}

	return true;
}

uint32_t Sdio::getBlocksNumber()
{
	uint8_t csd[16];
	uint8_t crc[2];

	{
		SdioChipSelectTrigger trigger(bsp::spi::Sdio);

		if (not waitNotBusy()) return 0;

		{
			static const uint8_t cmd[] = {
			    0x40 | 0x09 /* CMD9 */, 0x00,           0x00, 0x00,
			    0x00 /* ARG */,         (0x7F << 1) | 1 /* CRC7 + end bit */};
			spi().send(cmd, sizeof(cmd));
		}

		if (readByte() != 0x00) return 0;

		if (not waitDataToken(0xFE)) return 0;

		if (not readBytes(csd, sizeof(csd))) return 0;

		if (not readBytes(crc, sizeof(crc))) return 0;
	}

	if ((csd[0] & 0xC0) != 0x40) return 0;

	uint32_t result = csd[7] & 0x3F;  // two bits are reserved
	result = (result << 8) | csd[8];
	result = (result << 8) | csd[9];
	result = (result + 1) << 10;

	return result;
}


bool Sdio::readSingleBlock(uint32_t blockNum, uint8_t* buff)
{
	uint8_t crc[2];
	{
		SdioChipSelectTrigger trigger(bsp::spi::Sdio);

		if (not waitNotBusy()) return false;

		/* CMD17 (SEND_SINGLE_BLOCK) command */
		uint8_t cmd[] = {
		    0x40 | 0x11 /* CMD17 */, (blockNum >> 24) & 0xFF, /* ARG */
		    (blockNum >> 16) & 0xFF, (blockNum >> 8) & 0xFF,
		    blockNum & 0xFF,         (0x7F << 1) | 1 /* CRC7 + end bit */
		};
		spi().send(cmd, sizeof(cmd));

		if (readByte() != 0x00) return false;
		if (not waitDataToken(0xFE)) return false;
		if (not readBytes(buff, 512)) return false;
		if (not readBytes(crc, sizeof(crc))) return false;
	}

	return true;
}


bool Sdio::writeSingleBlock(uint32_t blockNum, const uint8_t* buff)
{
	{
		SdioChipSelectTrigger trigger(bsp::spi::Sdio);

		if (not waitNotBusy()) return false;

		/* CMD24 (WRITE_BLOCK) command */
		uint8_t cmd[] = {
		    0x40 | 0x18 /* CMD24 */, (blockNum >> 24) & 0xFF, /* ARG */
		    (blockNum >> 16) & 0xFF, (blockNum >> 8) & 0xFF,
		    blockNum & 0xFF,         (0x7F << 1) | 1 /* CRC7 + end bit */
		};

		spi().send(cmd, sizeof(cmd));

		if (not readByte() != 0x00) return false;

		uint8_t dataToken = 0xFE;
		uint8_t crc[2] = {0xFF, 0xFF};

		spi().send(&dataToken, sizeof(dataToken));
		spi().send(const_cast<uint8_t*>(buff), 512);
		spi().send(crc, sizeof(crc));

		/*
	   		dataResp:
	   		xxx0abc1
		   		010 - Data accepted
		   		101 - Data rejected due to CRC error
		   		110 - Data rejected due to write error
   		*/

	   uint8_t dataResp;
	   readBytes(&dataResp, sizeof(dataResp));
	   if ((dataResp & 0x1F) != 0x05) return false;
	   if (not waitNotBusy()) return false;
	}

	return true;
}

bool Sdio::readBegin(uint32_t blockNum)
{
	{
		SdioChipSelectTrigger trigger(bsp::spi::Sdio);

		if (not waitNotBusy()) return false;

		/* CMD18 (READ_MULTIPLE_BLOCK) command */
		uint8_t cmd[] = {
		    0x40 | 0x12 /* CMD18 */, (blockNum >> 24) & 0xFF, /* ARG */
		    (blockNum >> 16) & 0xFF, (blockNum >> 8) & 0xFF,
		    blockNum & 0xFF,         (0x7F << 1) | 1 /* CRC7 + end bit */
		};
		spi().send(cmd, sizeof(cmd));

		if (readByte() != 0x00) return false;
	}

	return true;
}


bool Sdio::readData(uint8_t* buff)
{
	uint8_t crc[2];
	{
		SdioChipSelectTrigger trigger(bsp::spi::Sdio);

		if (not waitDataToken(0xFE)) return false;
		if (not readBytes(buff, 512)) return false;
		if (not readBytes(crc, sizeof(crc))) return false;
	}

	return true;
}


bool Sdio::readEnd()
{
	{
		SdioChipSelectTrigger trigger(bsp::spi::Sdio);

		/* CMD12 (STOP_TRANSMISSION) */
		{
			static const uint8_t cmd[] = {
			    0x40 | 0x0C /* CMD12 */, 0x00,           0x00, 0x00,
			    0x00 /* ARG */,          (0x7F << 1) | 1};
			spi().send(cmd, sizeof(cmd));
		}

		uint8_t stuffByte;
		if (not readBytes(&stuffByte, sizeof(stuffByte))) return false;
		if (readByte() != 0x00) return false;
	}

	return true;
}


bool Sdio::writeBegin(uint32_t blockNum)
{
    {
		SdioChipSelectTrigger trigger(bsp::spi::Sdio);

		if (not waitNotBusy()) return false;

		/* CMD25 (WRITE_MULTIPLE_BLOCK) command */
		uint8_t cmd[] = {
		    0x40 | 0x19 /* CMD25 */, (blockNum >> 24) & 0xFF, /* ARG */
		    (blockNum >> 16) & 0xFF, (blockNum >> 8) & 0xFF,
		    blockNum & 0xFF,         (0x7F << 1) | 1 /* CRC7 + end bit */
		};
		spi().send(cmd, sizeof(cmd));

		if (readByte() != 0x00) return false;
	}

	return true;
}

bool Sdio::writeData(uint8_t* buff)
{
	{
		SdioChipSelectTrigger trigger(bsp::spi::Sdio);

		uint8_t dataToken = 0xFC;
		uint8_t crc[2] = {0xFF, 0xFF};
		spi().send(&dataToken, sizeof(dataToken));
		spi().send((uint8_t*)buff, 512);
		spi().send(crc, sizeof(crc));

		/*
		    dataResp:
		    xxx0abc1
		        010 - Data accepted
		        101 - Data rejected due to CRC error
		        110 - Data rejected due to write error
		*/
		uint8_t dataResp;
		readBytes(&dataResp, sizeof(dataResp));
		if ((dataResp & 0x1F) != 0x05) return false;

		if (not waitNotBusy()) return false;
	}

	return true;
}

bool Sdio::writeEnd()
{
	{
		SdioChipSelectTrigger trigger(bsp::spi::Sdio);

		uint8_t stopTran = 0xFD;
		spi().send(&stopTran, sizeof(stopTran));

		// skip one byte before readyng "busy"
		// this is required by the spec and is necessary for some real SD-cards!
		uint8_t skipByte;
		readBytes(&skipByte, sizeof(skipByte));

		if (not waitNotBusy()) return false;
	}

	return true;
}

DSTATUS Sdio::checkStatus()
{
	return 0;
}


DSTATUS Sdio::initialize()
{
	if (not init())
		return STA_NOINIT;
	
	return 0;
}


DRESULT Sdio::read(BYTE* buff, DWORD sector, UINT count)
{
	if (not readBegin(sector)) return RES_ERROR;

	while (count > 0) {
		if (not readData(buff)) return RES_ERROR;

		buff += 512;
		count--;
	}

	if (not readEnd()) return RES_ERROR;

	return RES_OK;
}

DRESULT Sdio::write(BYTE* buff, DWORD sector, UINT count)
{
	if (not writeBegin(sector)) return RES_ERROR;

	while (count > 0)
	{
		if (not writeData(buff)) return RES_ERROR;

		buff += 512;
		count--;
	}

	if (not writeEnd()) return RES_ERROR;

	return RES_OK;
}

DRESULT Sdio::ioctl(BYTE cmd, void* buff)
{
	if (cmd == CTRL_SYNC) {
		return RES_OK;
	} else {
		return RES_ERROR;
	}
}
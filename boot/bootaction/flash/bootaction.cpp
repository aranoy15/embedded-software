
#include <bootlcd.h>
#include <bootflash.h>
#include <bootinfo.h>
#include <cmath>
#include <string.h>

void bootAction()
{
	BootFlash flash;	

	flash.init();

	uint8_t header[3];

	flash.read(0, header, sizeof(header));

	uint8_t secretByte = header[0];
	uint16_t countPacket = header[1] | (header[2] << 8);

	if (secretByte == 0x87) {

		BootLcd::i2cInit();

		BootLcd lcd1(0x3F, 20, 4, 1);
		BootLcd lcd2(0x3B, 20, 4, 1);

		char message[64];

		uint32_t firmwareAddress = 1024;

		sprintf(message, "Finded new firmware");
		lcd1.sendString(message, strlen(message));

		BootInfo::unlock();

		BootInfo::eraseAppArea();

		for (uint32_t i = 0; i < countPacket; i++) {
			uint8_t packet[1024];

			uint32_t packetAddress = (i * 1024) + firmwareAddress;

			flash.read(packetAddress, packet, sizeof(packet));

			uint32_t offset = i * BootInfo::pageSize;

			uint8_t percent =  ceil(((i + 1) * 100.0f) / countPacket);
			sprintf(message, "Process: %3u", percent);
			lcd2.setCursor(0, 0);
			lcd2.sendString(message, strlen(message));
			lcd2.sendChar('%');

			BootInfo::writeBlock(BootInfo::appStartAddress + offset, packet, sizeof(packet));
		}

		sprintf(message, "%-20s", "Process: complete");
		lcd2.setCursor(0, 0);
		lcd2.sendString(message, strlen(message));

		BootInfo::lock();

		uint8_t empty[1] = {0};

		flash.writeData(0, empty, 1);
	}
}
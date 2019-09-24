
#include <bootuart.h>
#include <sleep.h>
#include <bootinfo.h>

static bool loadPackets(BootUart& uart)
{
	uint16_t packetNumber = 0;
	uint16_t packetCrc;
	uint8_t packet[1024];

	auto result = BootInfo::eraseAppArea();


	uart.putChar('!');

	while (true) {

		if (not uart.getBuf(reinterpret_cast<uint8_t *>(&packetNumber),
		                    sizeof(packetNumber)))
			return false;

		if (packetNumber == 0xFFFF) return true;

		if (not uart.getBuf(packet, sizeof(packet))) return false;
		if (not uart.getBuf(reinterpret_cast<uint8_t *>(&packetCrc),
		                    sizeof(packetCrc)))
			return false;

		uint16_t crc = BootInfo::crc16(packet, 1024);

		if (crc != packetCrc) return false;

		uint32_t offset = packetNumber * BootInfo::pageSize;

		if (not BootInfo::writeBlock(BootInfo::appStartAddress + offset, packet,
		                             1024))
			return false;

		sleep(10);
		uart.putChar('!');
	}
}

void bootAction()
{
	sleep(10);

	BootUart uart;
	uart.init();

	uart.putChar('>');

	if (uart.getChar() == '<') {
		BootInfo::unlock();
		bool result = loadPackets(uart);

		if (not result) uart.putChar('?');
		else uart.putChar('!');
		BootInfo::lock();
	}
}
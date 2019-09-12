#include <bootinfo.h>
//#include <bootuart.h>
#include <bootuart.h>
#include <bsp.h>
#include <sleep.h>



static bool loadPackets(BootUart& uart)
{
	uint16_t packetNumber = 0;
	uint16_t packetCrc;
	uint8_t packet[1024];

	auto result = BootInfo::eraseAppArea();


	//auto start = HAL_GetTick();
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

		/*
		if (not uart.getBuf(reinterpret_cast<uint8_t*>(&packet), 1028)) return false;

		packetNumber = *reinterpret_cast<uint16_t*>(&packet[0]);
		packetCrc = *reinterpret_cast<uint16_t*>(&packet[1026]);
		*/


		uint16_t crc = BootInfo::crc16(packet, 1024); //(&packet[2], 1024);

		if (crc != packetCrc) return false;

		uint32_t offset = packetNumber * BootInfo::pageSize;

		if (not BootInfo::writeBlock(BootInfo::appStartAddress + offset, packet,
		                             1024))
			return false;

		sleep(10);
		uart.putChar('!');
	}
}

static void bootAction(BootUart& uart)
{
	sleep(10);

	uart.putChar('>');

	if (uart.getChar() == '<') {
		BootInfo::unlock();
		bool result = loadPackets(uart);

		if (not result) uart.putChar('?');
		else uart.putChar('!');
		BootInfo::lock();
	}
}

int main(void)
{
	bsp::init();

	BootUart uart;
	uart.init();

	bootAction(uart);

	uart.puts_("Start application");

	__disable_irq(); // запрещаем прерывания

	SCB->VTOR = BootInfo::appStartAddress;
	uint32_t *int_vector_table = (uint32_t *)BootInfo::appStartAddress;

	typedef void (*ResetVectorFunc)();
	ResetVectorFunc resetVector = (ResetVectorFunc)int_vector_table[1];
	resetVector();
}

extern "C" {

void SysTick_Handler(void)
{
	HAL_IncTick();
}
}

#include <uart.hpp>
#include <bsp.hpp>

namespace
{

uint16_t crc16(const uint8_t* data, std::size_t size)
{
	uint16_t crc = 0;
	for (uint32_t i = 0; i < size; ++i) {
		crc ^= data[i];

		bool bit_moved = (crc & 1 << 15) != 0;
		crc <<= 1;

		if (bit_moved) crc |= 1;
	}

	return crc;
}

bool load_packets(lib::Uart& uart)
{
	uint16_t packet_number = 0;
	uint16_t packet_crc;
	uint8_t packet[1024];

	auto result = bsp::AppArea::full_erase();


	uart.put_char('!');

	while (true) {

		if (not uart.get_buf(reinterpret_cast<uint8_t *>(&packet_number),
		                    sizeof(packet_number)))
			return false;

		if (packet_number == 0xFFFF) return true;

		if (not uart.get_buf(packet, sizeof(packet))) return false;
		if (not uart.get_buf(reinterpret_cast<uint8_t *>(&packet_crc),
		                    sizeof(packet_crc)))
			return false;

		uint16_t crc = crc16(packet, 1024);

		if (crc != packet_crc) return false;

		uint32_t offset = packet_number * bsp::AppArea::page_size;

		if (not bsp::AppArea::write(bsp::AppArea::app_address + offset, packet,
		                            1024))
			return false;

		bsp::tick::delay(10);
		uart.put_char('!');
	}
}

static bool wait_start(lib::Uart& uart)
{
	for(uint8_t i = 0; i < 1; i++) {
		uart.put_char('>');

		if (uart.get_char() == '<') return true;

		bsp::tick::delay(10);
	}

	return false;
}
}

void boot_action()
{
    bsp::usart::init();

	bsp::tick::delay(10);

	lib::Uart uart;
	uart.init();

	if (wait_start(uart)) {
		bsp::AppArea::unlock();
		bool result = load_packets(uart);

		if (not result) uart.put_char('?');
		else uart.put_char('!');
		bsp::AppArea::lock();
	}
}
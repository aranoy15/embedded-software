
//#include <uart.hpp>
#include <bsp.hpp>
#include <wake.hpp>
#include <timer.h>

namespace
{

/*
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
*/

uint8_t buffer[lib::wake::Protocol::max_frame_size];
}

/*
typedef struct
{
  	char build[64];
	char hwver[16];
	char blver[16];
	char date[16];
	char time[16];
} TBootLoaderInfo;

const TBootLoaderInfo info __attribute__((section(".device_info"))) = {"Omnicomm Optim3", "HW: 021", "1.0.1.5", __DATE__, __TIME__};
*/

//char var_static_name[6] __attribute__((section(".device_info"))) = "ANTON";
//char var_static_last_name[9] __attribute__((section(".device_info"))) = "FEDOSEEV";

void boot_action()
{
    using protocol_t = lib::wake::Protocol;
    using packet_t = lib::wake::Packet;
    using timer_t = lib::timer::Timer;

    bsp::usart::init();
    bsp::usart::start_receive(buffer, sizeof(buffer));

    timer_t timer;

    timer.start();

    while(not timer.has_expired(timer_t ::minute(1))) {
        if (bsp::usart::is_idle()) {

            for (std::size_t i = 0; i < bsp::usart::count_receive(); ++i) {
                if (protocol_t::process(buffer[i])) {
                    packet_t packet = protocol_t::unpack();
                }
            }

            bsp::usart::restart_receive(buffer, sizeof(buffer));
        }
    }

	/*
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
	*/
}
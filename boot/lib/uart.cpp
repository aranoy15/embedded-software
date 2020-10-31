#include <lib/uart.hpp>
#include <bsp.hpp>
#include <string.h>

namespace lib
{

Uart::Uart() {}

void Uart::init()
{
    bsp::usart::init();
}

void Uart::put_char(char ch)
{
    put_buf((uint8_t*)&ch, sizeof(ch));
}

void Uart::puts(const char* str)
{
    put_buf((uint8_t*)str, strlen(str));
}

void Uart::puts_(const char* str)
{
    puts(str);
    puts("\r\n");
}

void Uart::put_buf(const uint8_t data[], std::size_t size)
{
    bsp::usart::send(data, size);
}

uint8_t Uart::get_char()
{
    uint8_t temp = 0;
    bsp::usart::receive(&temp, sizeof(temp));
    return temp;
}

std::size_t Uart::get_line(char* data, std::size_t size)
{
    uint8_t* temp_data = (uint8_t*)data;
	uint32_t count = 0;

	while (count < size) {
		char ch = get_char();
		if (ch == '\n' or ch == 0)
			break;

		*(temp_data++) = ch;
		count++;
	}

	return count;
}

bool Uart::get_buf(uint8_t data[], std::size_t size)
{
    return bsp::usart::receive(data, size);
}
}  // namespace lib

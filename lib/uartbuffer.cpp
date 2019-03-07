#include <uartbuffer.h>
#include <cstring>


UartBuffer::UartBuffer(uint16_t buffer_size)
    : m_buf(NULL), current_buffer_size(buffer_size)
{
	m_buf = new uint8_t[current_buffer_size];
	memset(m_buf, '\0', current_buffer_size);
}

uint16_t UartBuffer::CountSymbols()
{
	uint16_t count = 0;
	for (uint16_t i = 0; i < current_buffer_size; i++) {
		if (m_buf[i] == '\0')
			break;
		count++;
	}

	return count;
}

bool UartBuffer::GetFirstPacket(std::string *packet, uint8_t separator)
{
	uint16_t count_symbols = CountSymbols();

	if (count_symbols == 0) return false;

	uint16_t shift = 0;
	bool haveSeparator = false;
	for (uint16_t i = 0; i < count_symbols; i++) {
		shift++;

		if (m_buf[i] == separator) {
			haveSeparator = true;
			break;
		}

		/*
        if (i < size)
		    packet[i] = m_buf[i];
		*/
		packet->push_back(m_buf[i]);
	}

	if(haveSeparator) {
		m_buf = &m_buf[shift];
		memset(&m_buf[current_buffer_size - shift], '\0', shift);
	}

	return haveSeparator;
}
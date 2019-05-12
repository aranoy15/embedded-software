#include <uartbuffer.h>
#include <cstring>


UartBuffer::UartBuffer(uint16_t buffer_size)
    : m_buf(NULL), currentBufferSize(buffer_size)
{
	m_buf = new uint8_t[currentBufferSize];
	memset(m_buf, '\0', currentBufferSize);
}

uint16_t UartBuffer::countSymbols()
{
	uint16_t count = 0;
	for (uint16_t i = 0; i < currentBufferSize; i++) {
		if (m_buf[i] == '\0')
			break;
		count++;
	}

	return count;
}

bool UartBuffer::getFirstPacket(std::string *packet, uint8_t separator)
{
	uint16_t count_symbols = countSymbols();

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
		memset(&m_buf[currentBufferSize - shift], '\0', shift);
	}

	return haveSeparator;
}
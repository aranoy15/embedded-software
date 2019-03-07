#ifndef UARTBUFFER_H
#define UARTBUFFER_H

#include <stdint.h>
#include <stdio.h>
#include <string>

class UartBuffer
{
private:
	UartBuffer(const UartBuffer &);
	UartBuffer operator=(const UartBuffer &);

private:
	uint8_t *m_buf;
	uint16_t current_buffer_size;

public:
	UartBuffer(uint16_t buffer_size);
	virtual ~UartBuffer() { delete[] m_buf; }

	uint16_t CountSymbols();
	bool GetFirstPacket(std::string *packet, uint8_t separator);

	uint16_t Size() const { return current_buffer_size; }
	uint8_t *Buffer() { return m_buf; }
};

#endif /* UARTBUFFER_H */
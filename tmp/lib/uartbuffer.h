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
	uint16_t currentBufferSize;

public:
	UartBuffer(uint16_t buffer_size);
	virtual ~UartBuffer() { delete[] m_buf; }

	uint16_t countSymbols();
	bool getFirstPacket(std::string *packet, uint8_t separator);

	uint16_t size() const { return currentBufferSize; }
	uint8_t *buffer() { return m_buf; }
};

#endif /* UARTBUFFER_H */
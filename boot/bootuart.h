#ifndef BOOTLOADER_UART_H
#define BOOTLOADER_UART_H

#include <gpio.h>
#include <inttypes.h>

class BootUart
{
public:
	UART_HandleTypeDef m_huart;

public:
    BootUart();

    void init(uint32_t baudrate);
    void putChar(char ch);
    void puts(const char *str);
    void puts_(const char *str);
    void putBuf(const uint8_t *buf, uint32_t length);
    uint8_t getChar();
    uint32_t getLine(char *_data, uint32_t length);
    uint32_t getBuf(uint8_t *data, uint32_t length);
};

#endif /* BOOTLOADER_UART_H */

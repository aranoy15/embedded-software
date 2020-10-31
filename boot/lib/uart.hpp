#ifndef BOOT_LIB_UART
#define BOOT_LIB_UART

#include <inttypes.h>
#include <cstddef>

namespace lib
{
struct Uart
{
    Uart();

    void init();

    void put_char(char ch);
    void puts(const char* str);
    void puts_(const char* str);
    void put_buf(const uint8_t data[], std::size_t size);

    uint8_t get_char();
    std::size_t get_line(char* data, std::size_t size);
    bool get_buf(uint8_t data[], std::size_t size);
};
}

#endif /* BOOT_LIB_UART */

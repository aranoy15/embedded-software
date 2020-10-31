#ifndef BOOT_DRIVERS_BSP_F103XB_USART
#define BOOT_DRIVERS_BSP_F103XB_USART

#include <stdint.h>
#include <cstddef>

namespace bsp::usart
{
const uint32_t timeout = 1000;

void init();

void send(const uint8_t data[], std::size_t size);
bool receive(uint8_t data[], std::size_t size);
}

#endif /* BOOT_DRIVERS_BSP_F103XB_USART */

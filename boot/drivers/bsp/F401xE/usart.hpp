#ifndef BOOT_DRIVERS_BSP_F103XB_USART
#define BOOT_DRIVERS_BSP_F103XB_USART

#include <stdint.h>
#include <cstddef>

namespace bsp::usart
{
const uint32_t timeout = 1000;

void init();

void send(const uint8_t data[], std::size_t size);
//bool receive(uint8_t data[], std::size_t size);
bool start_receive(uint8_t data[], std::size_t size);
bool restart_receive(uint8_t data[], std::size_t size);
bool stop_receive();
std::size_t count_receive();
bool is_idle();
}

#endif /* BOOT_DRIVERS_BSP_F103XB_USART */

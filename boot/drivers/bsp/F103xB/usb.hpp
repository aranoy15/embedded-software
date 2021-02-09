#ifndef BOOT_DRIVERS_BSP_F103XB_USB
#define BOOT_DRIVERS_BSP_F103XB_USB

#include <cinttypes>
#include <cstddef>

namespace bsp::usb
{
void init();
bool send(const uint8_t data[], std::size_t size);
bool start_receive(uint8_t data[], std::size_t size);
bool restart_receive();
bool stop_receive();
std::size_t count_receive();
bool is_idle();
}

#endif /* BOOT_DRIVERS_BSP_F103XB_USB */

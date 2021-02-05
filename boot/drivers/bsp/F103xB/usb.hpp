#ifndef BOOT_DRIVERS_BSP_F103XB_USB
#define BOOT_DRIVERS_BSP_F103XB_USB

#include <cinttypes>
#include <cstddef>

namespace bsp::usb
{
void init();
bool send(const uint8_t data[], std::size_t size);
bool receive(const uint8_t data[], std::size_t size);
}

#endif /* BOOT_DRIVERS_BSP_F103XB_USB */

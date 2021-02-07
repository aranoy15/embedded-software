#ifndef BOOT_DRIVERS_BSP_F103XB_TICK
#define BOOT_DRIVERS_BSP_F103XB_TICK

#include <cinttypes>

namespace bsp::tick
{
    uint32_t current();
    void delay(uint32_t ms);
}

#endif /* BOOT_DRIVERS_BSP_F103XB_TICK */

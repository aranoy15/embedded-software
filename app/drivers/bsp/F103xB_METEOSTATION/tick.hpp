#ifndef DRIVERS_BSP_F103XB_METEOSTATION_TICK
#define DRIVERS_BSP_F103XB_METEOSTATION_TICK

#include <cinttypes>

namespace bsp::tick
{
std::uint32_t current();
void delay(std::uint32_t ms);
}

#endif /* DRIVERS_BSP_F103XB_METEOSTATION_TICK */

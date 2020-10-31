#ifndef DRIVERS_BSP_F103XB_METEOSTATION_BSP
#define DRIVERS_BSP_F103XB_METEOSTATION_BSP

#include <drivers/bsp/ST/bsp_template.hpp>
#include <stdint.h>

namespace bsp
{
void init();

namespace gpio
{
void init();

void status_on();
void status_off();
}

namespace tick
{
uint32_t current();
void delay(uint32_t ms);
}
}

#endif /* DRIVERS_BSP_F103XB_METEOSTATION_BSP */

#ifndef DRIVERS_BSP_F103XB_METEOSTATION_BSP
#define DRIVERS_BSP_F103XB_METEOSTATION_BSP

#include <bsp_template.hpp>
#include <tick.hpp>
#include <gpio.hpp>
#include <os.hpp>
#include <usart.hpp>
#include <wdt.hpp>

namespace bsp
{
void init();
void reset();
}

#endif /* DRIVERS_BSP_F103XB_METEOSTATION_BSP */

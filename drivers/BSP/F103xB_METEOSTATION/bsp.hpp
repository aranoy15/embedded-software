#ifndef DRIVERS_BSP_F103XB_METEOSTATION_BSP
#define DRIVERS_BSP_F103XB_METEOSTATION_BSP

#include <drivers/bsp/ST/bsp_template.hpp>
#include <drivers/bsp/F103xB_METEOSTATION/tick.hpp>
#include <drivers/bsp/F103xB_METEOSTATION/gpio.hpp>
#include <drivers/bsp/F103xB_METEOSTATION/usart.hpp>
#include <drivers/bsp/F103xB_METEOSTATION/i2c.hpp>
#include <drivers/bsp/F103xB_METEOSTATION/iwdg.hpp>

namespace bsp
{
void init();
void reset();
}

#endif /* DRIVERS_BSP_F103XB_METEOSTATION_BSP */

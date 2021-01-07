#ifndef DRIVERS_BSP_F103XB_METEOSTATION_BSP
#define DRIVERS_BSP_F103XB_METEOSTATION_BSP

#include <drivers/bsp/ST/bsp_template.hpp>
#include <drivers/bsp/F401xE_TEST/tick.hpp>
#include <drivers/bsp/F401xE_TEST/gpio.hpp>
#include <drivers/bsp/F401xE_TEST/os.hpp>
#include <drivers/bsp/F401xE_TEST/usart.hpp>

namespace bsp
{
void init();
void reset();
}

#endif /* DRIVERS_BSP_F103XB_METEOSTATION_BSP */

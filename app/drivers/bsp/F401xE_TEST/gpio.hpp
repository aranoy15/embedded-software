#ifndef DRIVERS_BSP_F103XB_METEOSTATION_GPIO
#define DRIVERS_BSP_F103XB_METEOSTATION_GPIO

namespace bsp::gpio
{
void init();

namespace status
{
void on();
void off();
void toggle();
}

namespace gsm
{
void on();
void off();
}
}

#endif /* DRIVERS_BSP_F103XB_METEOSTATION_GPIO */

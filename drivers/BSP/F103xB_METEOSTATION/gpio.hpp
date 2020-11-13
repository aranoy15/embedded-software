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

namespace red
{
void on();
void off();
void toggle();
}

namespace blue
{
void on();
void off();
void toggle();
}

namespace green
{
void on();
void off();
void toggle();
}
}

#endif /* DRIVERS_BSP_F103XB_METEOSTATION_GPIO */

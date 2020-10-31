#include <drivers/bsp/F103xB/tick.hpp>
#include <main.h>

void bsp::tick::delay(uint32_t ms)
{
    HAL_Delay(ms);
}

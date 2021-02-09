#include <drivers/bsp/F103xB/tick.hpp>
#include <main.h>

uint32_t bsp::tick::current()
{
    return HAL_GetTick();
}

void bsp::tick::delay(uint32_t ms)
{
    HAL_Delay(ms);
}

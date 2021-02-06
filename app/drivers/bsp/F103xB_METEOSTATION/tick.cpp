#include <tick.hpp>
#include <main.h>

std::uint32_t bsp::tick::current()
{
    return HAL_GetTick();
}

void bsp::tick::delay(std::uint32_t ms)
{
    HAL_Delay(ms);
}

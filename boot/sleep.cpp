#include <bsp.h>

#include <sleep.h>

void sleep(uint32_t ms)
{
    uint32_t s = HAL_GetTick();
    while (HAL_GetTick() - s < ms);
}
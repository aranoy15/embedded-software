#include <tick.hpp>
#include <main.h>
#include <cmsis_os2.h>

std::uint32_t bsp::tick::current()
{
    if (osKernelGetState() == osKernelRunning) {
        return osKernelGetTickCount();
    } else {
        return HAL_GetTick();
    }
}

void bsp::tick::delay(std::uint32_t ms)
{
    if (osKernelGetState() == osKernelRunning) {
        osDelay(ms);
    } else {
        HAL_Delay(ms);
    }
}

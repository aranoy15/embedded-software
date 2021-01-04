#include <drivers/bsp/F401xE_TEST/tick.hpp>
#include <drivers/F401xE_TEST/Core/Inc/main.h>
#include <drivers/F401xE_TEST/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/cmsis_os2.h>

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

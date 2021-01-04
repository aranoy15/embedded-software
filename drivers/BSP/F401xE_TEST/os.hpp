#ifndef DRIVERS_BSP_F401XE_TEST_OS
#define DRIVERS_BSP_F401XE_TEST_OS

#include <drivers/F401xE_TEST/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/cmsis_os2.h>
#include <string>

namespace bsp::os
{
    using func_t = osThreadFunc_t;
    using handle_t = osThreadId_t;

    enum class Priority
    {
        Idle = osPriorityIdle,
        Low = osPriorityLow,
        Normal = osPriorityNormal,
        High = osPriorityHigh,
        Realtime = osPriorityRealtime
    };

    void init();
    void start();
    handle_t reg(func_t func, void* args, std::string_view name, std::size_t stack, Priority prio);
}

#endif /* DRIVERS_BSP_F401XE_TEST_OS */

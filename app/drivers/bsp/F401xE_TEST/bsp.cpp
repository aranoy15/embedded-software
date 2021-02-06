#include <bsp.hpp>
#include <main.h>
#include <gpio.h>

extern "C" {
void SystemClock_Config(void);
}

namespace bsp
{
void init()
{
    HAL_Init();

    SystemClock_Config();
}

void reset()
{
    NVIC_SystemReset();
}
}

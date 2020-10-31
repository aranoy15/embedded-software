#include <drivers/bsp/F103xB/bsp.hpp>
#include <main.h>

extern "C" {
void SystemClock_Config(void);
}

void bsp::init()
{
    HAL_Init();

    SystemClock_Config();
}


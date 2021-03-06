#include <bsp.hpp>
#include <main.h>
#include <gpio.h>

extern "C" {
void SystemClock_Config(void);
}


void bsp::init()
{
    HAL_Init();

    SystemClock_Config();
}

void bsp::reset()
{
    NVIC_SystemReset();
}

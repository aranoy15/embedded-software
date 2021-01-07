#include <drivers/bsp/F103xB_METEOSTATION/bsp.hpp>
#include <drivers/csp/F401xE_TEST/Core/Inc/main.h>
#include <drivers/csp/F401xE_TEST/Core/Inc/gpio.h>

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

#include <drivers/bsp/F401xE_TEST/wdt.hpp>
#include <iwdg.h>

void bsp::wdt::init()
{
#if (!IS_DEBUG)
    MX_IWDG_Init();
#endif
}

void bsp::wdt::reset()
{
#if (!IS_DEBUG)
    HAL_IWDG_Refresh(&hiwdg);
#endif
}

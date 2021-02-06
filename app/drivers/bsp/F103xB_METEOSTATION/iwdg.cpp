#include <iwdg.hpp>
#include <iwdg.h>

void bsp::iwdg::init()
{
#if !DEBUG
	MX_IWDG_Init();
#endif
}

void bsp::iwdg::reset()
{
#if !DEBUG
    HAL_IWDG_Refresh(&hiwdg);
#endif
}

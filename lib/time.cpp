#include <time.h>

void Time::Sleep(const Time& t)
{
	if (t.m_time <= 0) return;

	#if NEED_FREERTOS
	if (osKernelRunning())
		osDelay(t.m_time);
	else
	#endif
		HAL_Delay(t.m_time);
}

uint32_t Time::GetTicks()
{
	#if NEED_FREERTOS
	if(osKernelRunning())
		return osKernelSysTick();
	else
	#endif
		return HAL_GetTick();
}
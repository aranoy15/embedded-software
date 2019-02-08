#include <time.h>

void Time::Sleep(const Time& t)
{
	if (t.m_time <= 0) return;

	if (osKernelRunning())
		osDelay(t.m_time);
	else
		HAL_Delay(t.m_time);
}

uint32_t Time::GetTicks()
{
	if(osKernelRunning())
		return osKernelSysTick();
	else
		return HAL_GetTick();
}
#include <time.h>
#include <cmsis_os.h>

void Time::sleep(const Time& t)
{
	if (t.m_time <= 0) return;

	if (osKernelRunning())
		osDelay(t.m_time);
	else
		HAL_Delay(t.m_time);
}

uint32_t Time::getTicks()
{
	if(osKernelRunning())
		return osKernelSysTick();
	else
		return HAL_GetTick();
}
#include <time.h>
#include <bsp.h>
#include <os.h>

void Time::sleep(const Time& t)
{
	if (t.m_time <= 0) return;

	/*
	if (osKernelRunning())
		osDelay(t.m_time);
	else
	*/
	//HAL_Delay(t.m_time);

#if (USE_FREERTOS)
	if (osRunning()) vTaskDelay(t.m_time / portTICK_PERIOD_MS);
#else
	HAL_Delay(t.m_time);
#endif
}

uint32_t Time::getTicks()
{
	/*
	if(osKernelRunning())
		return osKernelSysTick();
	else
	*/
	return HAL_GetTick();
}
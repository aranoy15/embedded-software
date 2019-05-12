#include <time.h>

void Time::sleep(const Time& t)
{
	if (t.m_time <= 0) return;

	HAL_Delay(t.m_time);
}

uint32_t Time::getTicks()
{
	return HAL_GetTick();
}
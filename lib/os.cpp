#include <os.h>

#include <bsp.h>

bool osRunning()
{
#if (USE_FREERTOS)
	return xTaskGetSchedulerState() == taskSCHEDULER_RUNNING;
#else
	return false;
#endif
}
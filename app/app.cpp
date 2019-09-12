#include <stdint.h>
#include <bsp.h>
#include <logic.h>

#if (USE_FREERTOS)
#include <FreeRTOS.h>
#include <task.h>

extern "C" void* malloc(size_t size)
{
	return pvPortMalloc(size);
}

extern "C" void free(void* block)
{
	return vPortFree(block);
}

void* operator new(size_t size)
{
	void* block = malloc(size);
	return block;
}

void operator delete(void* block) 
{
	free(block);
}

void operator delete(void* block, size_t size) 
{
	(void)size;
	free(block);
}
#endif

__attribute__((constructor)) void initAll()
{
	bsp::init();

#if (USE_FREERTOS)
	bsp::initMem();
#endif
}

#if (USE_FREERTOS)
void start(void*)
{
	applogic::startLogic();
}
#endif

int main()
{
#if (USE_FREERTOS)
	xTaskCreate(start, "main", (2 * 1024)/ sizeof(size_t), NULL, 10, NULL);
	vTaskStartScheduler();
#else
	applogic::startLogic();
#endif
}

extern "C" {

extern void xPortSysTickHandler(void);

void SysTick_Handler(void)
{
	HAL_IncTick();
	//osSystickHandler();

#if (USE_FREERTOS)
#if (INCLUDE_xTaskGetSchedulerState == 1)
	if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED) {
#endif /* INCLUDE_xTaskGetSchedulerState */
		xPortSysTickHandler();
#if (INCLUDE_xTaskGetSchedulerState == 1)
	}
#endif
#endif
}
}
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

/*
static const size_t mainRamHeapSize = 17 * 1024;
static uint8_t mainRamHeapBlock[mainRamHeapSize];
HeapRegion_t xHeapRegions[] = {{mainRamHeapBlock, mainRamHeapSize},
                               {NULL, 0}};

void initMem()
{
	vPortDefineHeapRegions(xHeapRegions);
}
*/

__attribute__((constructor))
void initAll()
{
	//stm32system::init();
	bsp::init();
	//initMem();
}

/*
void start(const void*)
{
	MainThread mainThread;	
	mainThread.threadFunc();
}
*/

int main()
{
	//osThreadDef_t mainThread = {const_cast<char*>(std::string("main").c_str()), start, osPriorityNormal, 0, (2 * 1024) / sizeof(size_t)};	
	//osThreadCreate(&mainThread, NULL);
	//osKernelStart();

	applogic::startLogic();
}

extern "C" {
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
#include <stdint.h>
#include <stm32f1xx_hal.h>
#include <rcc.h>
#include <gpio.h>
#include <watchdog.h>


#include <singleton.h>
#include <time.h>
#include <uart.h>
#include <uartbuffer.h>
#include <string.h>
#include <i2c.h>
#include <mainthread.h>
#include <cmsis_os.h>

#include <vector>

using namespace gpio;
using namespace uart;

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

void operator delete(void* block, std::size_t size) 
{
	(void)size;
	free(block);
}

static const size_t mainRamHeapSize = 14 * 1024;
static uint8_t mainRamHeapBlock[mainRamHeapSize];
HeapRegion_t xHeapRegions[] = {{mainRamHeapBlock, mainRamHeapSize},
                               {NULL, 0}};

void initMem()
{
	vPortDefineHeapRegions(xHeapRegions);
}

__attribute__((constructor))
void initAll()
{
	HAL_Init();
	SystemInit();
	rcc::Init();
	initMem();
}

void start(const void*)
{
	MainThread mainThread;	
	mainThread.threadFunc();
}

int main()
{
	osThreadDef_t mainThread = {const_cast<char*>(std::string("main").c_str()), start, osPriorityNormal, 0, (2 * 1024) / sizeof(size_t)};	
	osThreadCreate(&mainThread, NULL);

	osKernelStart();
}

extern "C" {
void SysTick_Handler(void)
{
	HAL_IncTick();
	osSystickHandler();
}
}
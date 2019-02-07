//#include <gpio.h>
#include <lib/rcc.h>
#include <lib/watchdog.h>
#include <lib/gpio.h>
#include <tuple>
#include <vector>

#include <cmsis_os.h>

using namespace gpio;

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
	return malloc(size);
}


void operator delete(void* block)
{
	free(block);
}

typedef GPIO<PinDef<CSP_GPIO_PORT_NBR_C, GPIO_PIN_13>, mOutputPP> ErrorNormal;

__attribute__((constructor))
void initAll()
{
	HAL_Init();
	rcc::Init();
}

void testThread(const void *arguments)
{
	ErrorNormal::Setup();
	for(;;)	{
		ErrorNormal::On();
		osDelay(500);
		ErrorNormal::Off();
		osDelay(500);
	}
}

int main()
{
	osThreadDef(testThread, testThread, osPriorityNormal, 0, 512);	
	osThreadCreate(osThread(testThread), NULL);
	osKernelStart();

	//ErrorNormal::Setup();
	//for(;;) {
	//	ErrorNormal::On();
	//	HAL_Delay(500);
	//	ErrorNormal::Off();
	//	HAL_Delay(500);
	//}
}

extern "C" {
void SysTick_Handler(void)
{
	HAL_IncTick();
	osSystickHandler();
}
}
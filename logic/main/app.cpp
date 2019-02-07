#include <stdint.h>
#include <stm32f1xx_hal.h>
#include <rcc.h>
#include <gpio.h>
#include <watchdog.h>
#include <cmsis_os.h>

using namespace gpio;

void* malloc(size_t size)
{
	return pvPortMalloc(size);
}


void free(void* block)
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

__attribute__((constructor))
void InitAll()
{
	HAL_Init();
	rcc::Init();
}

void blinkTask(const void *data)
{
	typedef GPIO<PinDef<CSP_GPIO_PORT_NBR_C, GPIO_PIN_13>, mOutputPP> ErrorNormal;
	ErrorNormal::Setup();

	for(;;) {
		ErrorNormal::On();
		osDelay(250);
		ErrorNormal::Off();
		osDelay(250);
	}
}

void sendTask(const void *data)
{
	for(;;) {
		osDelay(1);
	}
}

int main()
{
	osThreadDef(blink, blinkTask, osPriorityNormal, 0, 2 * 1024 /sizeof(size_t));
	osThreadCreate(osThread(blink), NULL);

	osThreadDef(send, sendTask, osPriorityNormal, 0, 1 * 1024 / sizeof(size_t));
	osThreadCreate(osThread(send), NULL);

	Watchdog::Init();
	Watchdog::Start();
	Watchdog::Reload();

	osKernelStart();
}

extern "C" {
void SysTick_Handler(void)
{
	HAL_IncTick();
	osSystickHandler();
}
}
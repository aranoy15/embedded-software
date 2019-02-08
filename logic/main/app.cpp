#include <stdint.h>
#include <stm32f1xx_hal.h>
#include <rcc.h>
#include <gpio.h>
#include <watchdog.h>
#include <cmsis_os.h>

#include <thread.h>
#include <mutex.h>
#include <singleton.h>
#include <time.h>

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

class BlinkThread : public Thread, public Singleton<BlinkThread>
{
public:
	BlinkThread() : Thread(osPriorityNormal, 1024, "blink") {}

	virtual void ThreadFunc()
	{
		typedef GPIO<PinDef<CSP_GPIO_PORT_NBR_C, GPIO_PIN_13>, mOutputPP>
		    ErrorNormal;
		ErrorNormal::Setup();

		Watchdog::Init();
		Watchdog::Start();
		Watchdog::Reload();
		Mutex mutex;

		for (;;) {
			Lock lock(mutex);
			ErrorNormal::On();
			Time::Sleep(Time(250));
			ErrorNormal::Off();
			Time::Sleep(Time(250));

			Watchdog::Reload();
		}
	}
};

void mainThread(const void* arguments)
{
	BlinkThread::Instance()->Start();

	for(;;){
		Time::Sleep(Time(1));
	}
}

int main()
{
	osThreadDef(main, mainThread, osPriorityNormal, 0, 1024);
	osThreadCreate(osThread(main), NULL);

	osKernelStart();
}

extern "C" {
void SysTick_Handler(void)
{
	HAL_IncTick();
	osSystickHandler();
}
}
#include <stdint.h>
#include <stm32f1xx_hal.h>
#include <rcc.h>
#include <gpio.h>
#include <watchdog.h>

#if NEED_FREERTOS
#include <cmsis_os.h>
#include <thread.h>
#include <mutex.h>
#endif

#include <singleton.h>
#include <time.h>
#include <uart.h>
#include <uartbuffer.h>

#include <vector>

using namespace gpio;
using namespace uart;

#if NEED_FREERTOS
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
#endif

__attribute__((constructor))
void InitAll()
{
	HAL_Init();
	rcc::Init();
	Uart<usart1>::instance()->init(512, 115200);
}

#if NEED_FREERTOS
class BlinkThread : public Thread, public Singleton<BlinkThread>
{
public:
	BlinkThread() : Thread(osPriorityNormal, 1024, "blink") {}

	void ThreadFunc() override
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

class MessageThread : public Thread
{
public:
	MessageThread() : Thread(osPriorityNormal, 1024, "send") {}

	void ThreadFunc() override 
	{
		typedef Uart<usart1> debug;
		debug::instance()->init(128, 115200);

		for(;;) {
			debug::instance()->send("Hello World!");
			Time::Sleep(Time::Seconds(1));
		}
	}
};

void mainThread(const void* arguments)
{
	BlinkThread::instance()->Start();
	(new MessageThread())->Start();

	for(;;){
		Time::Sleep(Time(1));
	}
}
#endif

int main()
{
	#if NEED_FREERTOS
	osThreadDef(main, mainThread, osPriorityNormal, 0, 1024);
	osThreadCreate(osThread(main), NULL);

	osKernelStart();
	#endif

	typedef GPIO<PinDef<CSP_GPIO_PORT_NBR_C, GPIO_PIN_13>, mOutputPP>
		    ErrorNormal;
	ErrorNormal::Setup();

	typedef Uart<usart1> debug;
	debug::instance()->init(128, 115200);

	Watchdog::Init();
	Watchdog::Start();
	Watchdog::Reload();

	bool result = false;
	std::string message;

	for (;;) {
		ErrorNormal::On();
		Time::Sleep(Time(250));
		ErrorNormal::Off();
		Time::Sleep(Time(250));

		debug::instance()->send("Hello World!");

		std::tie(result, message) = debug::instance()->read();

		if (result)
			debug::instance()->send("Have message");
	}
}

extern "C" {
void SysTick_Handler(void)
{
	HAL_IncTick();

	#if NEED_FREERTOS
	osSystickHandler();
	#endif
}
}
//#include <gpio.h>
#include <lib/rcc.h>
#include <lib/watchdog.h>
#include <lib/gpio.h>

using namespace gpio;

typedef GPIO<PinDef<CSP_GPIO_PORT_NBR_C, GPIO_PIN_13>, mOutputPP> ErrorNormal;
typedef GPIO<PinDef<CSP_GPIO_PORT_NBR_C, GPIO_PIN_14>, mOutputPP> Test;

int main()
{
	HAL_Init();
	rcc::Init();

	ErrorNormal::Setup();
	Test::Setup();
	Watchdog::Init();
	Watchdog::Start();
	Watchdog::Reload();

	while (true) {
		ErrorNormal::On();
		Test::On();
		HAL_Delay(500);
		ErrorNormal::Off();
		Test::Off();
		HAL_Delay(500);

		Watchdog::Reload();
	}
}

extern "C" {
void SysTick_Handler(void)
{
	HAL_IncTick();
}
}
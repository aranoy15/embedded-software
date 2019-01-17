#include <gpio.h>
#include <rcc.h>
#include <watchdog.h>

using namespace gpio;

typedef GPIO<PinDef<CSP_GPIO_PORT_NBR_C, GPIO_PIN_13>, mOutputPP> ErrorNormal;

int main()
{
	HAL_Init();
	rcc::Init();
	ErrorNormal::Setup();
	Watchdog::Init();
	Watchdog::Start();
	Watchdog::Reload();

	while (true) {
		ErrorNormal::On();
		HAL_Delay(500);
		ErrorNormal::Off();
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
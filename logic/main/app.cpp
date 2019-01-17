//#include <gpio.h>
#include <rcc.h>
#include <watchdog.h>
#include <usbdevice.h>
#include <gpio.h>

using namespace gpio;

typedef GPIO<PinDef<CSP_GPIO_PORT_NBR_C, GPIO_PIN_13>, mOutputPP> ErrorNormal;
typedef GPIO<PinDef<CSP_GPIO_PORT_NBR_C, GPIO_PIN_14>, mOutputPP> Test;

int main()
{
	HAL_Init();
	rcc::Init();
	usb::UsbDeviceInit();

	ErrorNormal::Setup();
	Test::Setup();
	Watchdog::Init();
	Watchdog::Start();
	Watchdog::Reload();

	uint8_t message[] = "HI, ANTON!";
	uint8_t recieved[256];
	uint32_t length = 0;

	while (true) {
		ErrorNormal::On();
		Test::On();
		HAL_Delay(500);
		ErrorNormal::Off();
		Test::Off();
		HAL_Delay(500);

		//if (usb::ReceiveMessage(recieved, &length))
			usb::SendMessage(message, strlen((const char*)message));
		Watchdog::Reload();
	}
}

extern "C" {
void SysTick_Handler(void)
{
	HAL_IncTick();
}

extern PCD_HandleTypeDef hpcd_USB_FS;
void USB_LP_CAN1_RX0_IRQHandler(void)
{
  HAL_PCD_IRQHandler(&hpcd_USB_FS);
}
}
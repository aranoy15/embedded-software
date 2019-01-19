#include <stdint.h>
#include <app.h>
#include <gpio.h>
#include <rcc.h>

//extern PCD_HandleTypeDef hpcd_USB_FS;

int main()
{
	HAL_Init();
	RccInit();
	GpioSetup(CSP_GPIO_PORT_NBR_C, GPIO_PIN_13, GPIO_MODE_OUTPUT_PP, GPIO_SPEED_FREQ_HIGH, GPIO_NOPULL);

	/*
	uint8_t message[] = "HI, ANTON!";
	uint8_t recieved[256];
	uint32_t length = 0;
	*/

	for (;;) {
		GpioOn(CSP_GPIO_PORT_NBR_C, GPIO_PIN_13);
		HAL_Delay(500);
		GpioOff(CSP_GPIO_PORT_NBR_C, GPIO_PIN_13);
		HAL_Delay(500);
	}
}

void SysTick_Handler(void)
{
	HAL_IncTick();
}

void USB_LP_CAN1_RX0_IRQHandler(void)
{
  //HAL_PCD_IRQHandler(&hpcd_USB_FS);
}
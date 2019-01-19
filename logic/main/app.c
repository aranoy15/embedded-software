#include <stdint.h>
#include <stm32f1xx_hal.h>
#include <gpio.h>
#include <rcc.h>
#include <usbdevice.h>
#include <cmsis_os.h>

void* malloc(size_t size)
{
	return pvPortMalloc(size);
}


void free(void* block)
{
	return vPortFree(block);
}

extern PCD_HandleTypeDef hpcd_USB_FS;

__attribute__((constructor))
void InitAll()
{
	HAL_Init();
	RccInit();
	UsbDeviceInit();
}

void blinkTask(void *data)
{
	GpioSetup(CSP_GPIO_PORT_NBR_C, GPIO_PIN_13, GPIO_MODE_OUTPUT_PP, GPIO_SPEED_FREQ_HIGH, GPIO_NOPULL);

	for (;;) {
		GpioOn(CSP_GPIO_PORT_NBR_C, GPIO_PIN_13);
		osDelay(250);
		GpioOff(CSP_GPIO_PORT_NBR_C, GPIO_PIN_13);
		osDelay(250);
	}
}

void sendTask(void *data)
{
	uint8_t message[] = "HI, ANTON!\n";
	
	for(;;) {
		CDC_Transmit_FS(message, strlen((const char*)message));
		osDelay(2000);
	}
}

int main()
{
	osThreadDef(send, sendTask, osPriorityNormal, 0, 3 * 1024 / sizeof(size_t));
	osThreadCreate(osThread(send), NULL);

	osThreadDef(blink, blinkTask, osPriorityNormal, 0, 1024 / sizeof(size_t));
	osThreadCreate(osThread(blink), NULL);

	osKernelStart();
}

void SysTick_Handler(void)
{
	HAL_IncTick();
	osSystickHandler();
}

void USB_LP_CAN1_RX0_IRQHandler(void)
{
  HAL_PCD_IRQHandler(&hpcd_USB_FS);
}
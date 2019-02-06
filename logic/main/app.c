#include <stdint.h>
#include <stm32f1xx_hal.h>
#include <gpio.h>
#include <rcc.h>
#include <usbdevice.h>
#include <watchdog.h>
#include <cmsis_os.h>
#include <i2c.h>
#include <string.h>

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
	I2cInit();
}

void blinkTask(void *data)
{
	GpioSetup(CSP_GPIO_PORT_NBR_C, GPIO_PIN_13, GPIO_MODE_OUTPUT_PP, GPIO_SPEED_FREQ_HIGH, GPIO_NOPULL);

	for (;;) {
		GpioOn(CSP_GPIO_PORT_NBR_C, GPIO_PIN_13);
		osDelay(250);
		GpioOff(CSP_GPIO_PORT_NBR_C, GPIO_PIN_13);
		osDelay(250);
		WatchdogReload();
	}
}

void sendTask(void *data)
{
	char info[] = "Scanning I2C bus...\r\n";
	
	for(;;) {
		CDC_Transmit_FS(info, strlen(info));

		HAL_StatusTypeDef res;
		for (uint16_t i = 0; i < 128; i++) {
			res = HAL_I2C_IsDeviceReady(&hi2c1, i << 1, 1, 10);
			if (res == HAL_OK) {
				//char msg[64];
				//sprintf(msg, "0x%02X", i);
				CDC_Transmit_FS("OK", strlen("OK"));
			} else {
				CDC_Transmit_FS((uint8_t*)".", 1);
			}
		}

		CDC_Transmit_FS((uint8_t*)"\r\n", 2);

		osDelay(5000);
	}
}

int main()
{
	osThreadDef(send, sendTask, osPriorityNormal, 0, 3 * 1024 / sizeof(size_t));
	osThreadCreate(osThread(send), NULL);

	osThreadDef(blink, blinkTask, osPriorityNormal, 0, 1024 / sizeof(size_t));
	osThreadCreate(osThread(blink), NULL);

	WatchdogInit();
	WatchdogStart();
	WatchdogReload();

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
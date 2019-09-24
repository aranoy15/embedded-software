#include <bsp.h>
#include <usb.h>
#include <fatfsclass.h>

extern "C" {

//extern PCD_HandleTypeDef hpcd_USB_FS;

void USB_LP_CAN1_RX0_IRQHandler(void) 
{
    //HAL_PCD_IRQHandler(&hpcd_USB_FS);
    UsbDevice::instance()->irqHandler();
}

void EXTI9_5_IRQHandler(void)
{
    bool state = FatFsClass::instance()->cardDetected();
    if (state)
	    FatFsClass::instance()->mHaveNewCard = true;
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_8);
}
}
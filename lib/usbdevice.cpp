#include <usbdevice.h>

USBD_HandleTypeDef hUsbDeviceFS;

void usb::UsbDeviceInit()
{
	USBD_Init(&hUsbDeviceFS, &FS_Desc, DEVICE_FS);
	USBD_RegisterClass(&hUsbDeviceFS, &USBD_CDC);
	USBD_CDC_RegisterInterface(&hUsbDeviceFS, &USBD_Interface_fops_FS);
	USBD_Start(&hUsbDeviceFS);
}

bool usb::SendMessage(uint8_t* buf, uint16_t len)
{
    uint8_t result = CDC_Transmit_FS(buf, len);
    return result == USBD_OK;
}

bool usb::ReceiveMessage(uint8_t* buf, uint32_t *len)
{
    //uint8_t result = CDC_Receive_FS(buf, len);
    uint8_t result = CDC_Receive_FS(buf, len);
    return result == USBD_OK;
}
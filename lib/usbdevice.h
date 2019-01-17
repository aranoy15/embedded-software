#pragma once

#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_cdc.h"
#include "usbd_cdc_if.h"

namespace usb
{
    void UsbDeviceInit();
    bool SendMessage(uint8_t* buf, uint16_t len);
    bool ReceiveMessage(uint8_t* buf, uint32_t *len);
}
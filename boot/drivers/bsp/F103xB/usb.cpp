#include <usb.hpp>
#include <usb_device.h>
#include <usbd_cdc_if.h>

namespace bsp::usb
{
void init()
{
    MX_USB_DEVICE_Init();
}

bool send(const uint8_t data[], std::size_t size)
{
    uint8_t status = CDC_Transmit_FS(const_cast<uint8_t*>(data), size);
    return status == USBD_OK;
}

bool receive(const uint8_t data[], std::size_t size)
{
    return false;
}
}


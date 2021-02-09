#include <usb.hpp>
#include <usb_device.h>
#include <usbd_cdc_if.h>

namespace
{
struct ReceiveData
{
    uint8_t* buffer = nullptr;
    std::size_t size = 0;
    std::size_t count = 0;
    bool is_started = false;
    bool is_idle = false;
};

ReceiveData rx_data;

}

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

bool start_receive(uint8_t data[], std::size_t size)
{
    rx_data.buffer = data;
    rx_data.size = size;
    rx_data.is_started = true;
}

bool restart_receive()
{
    rx_data.count = 0;
    rx_data.is_started = true;
    rx_data.is_idle = false;
}

bool stop_receive()
{
    rx_data.count = 0;
    rx_data.is_started = false;
    rx_data.is_idle = false;
}

std::size_t count_receive()
{
    return rx_data.count;
}

bool is_idle()
{
    return rx_data.is_idle;
}
}

extern "C" {
void USB_Receive_FS_Callback(uint8_t* Buf, uint16_t Len) 
{
    rx_data.is_idle = true;
}
}

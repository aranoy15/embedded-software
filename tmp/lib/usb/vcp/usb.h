#pragma once

#include <bsp.h>
#include <singleton.h>
#include <string>

#include <usbd_def.h>
#include <usbd_core.h>
#include <usbd_cdc.h>

#include <mutex.h>
#include <cyclicbuffer.h>
#include <timer.h>

extern "C" void USB_LP_CAN1_RX0_IRQHandler(void);
extern "C" USBD_StatusTypeDef USBD_LL_Init(USBD_HandleTypeDef *pdev);

class UsbDevice : public Singleton<UsbDevice>
{
    friend void USB_LP_CAN1_RX0_IRQHandler(void);
    friend USBD_StatusTypeDef USBD_LL_Init(USBD_HandleTypeDef *pdev);

private:


    static USBD_CDC_ItfTypeDef interfaceFs;

    static constexpr uint32_t rxSize = 2048;
    static constexpr uint32_t txSize = 1024;

    static uint8_t mRxBuffer[rxSize];
    static uint8_t mTxBuffer[txSize];


private:
    USBD_HandleTypeDef mHusb;
    PCD_HandleTypeDef mHpcd;

    MutexRe mMutex;
    CyclicBuffer<uint8_t> mData;

public:
    static constexpr uint16_t USBD_VID = 1155;
    static constexpr uint16_t USBD_LANGID_STRING = 1033;
    static const char* USBD_MANUFACTURER_STRING;
    static constexpr uint16_t USBD_PID_FS = 22336;
    static const char* USBD_PRODUCT_STRING_FS;
    static const char* USBD_CONFIGURATION_STRING_FS;
    static const char* USBD_INTERFACE_STRING_FS;

    static constexpr uint32_t USB_SIZ_STRING_SERIAL = 0x1A;

private:
    static USBD_DescriptorsTypeDef fsDesc;

    static constexpr uint32_t DEVICE_ID1 = UID_BASE;
    static constexpr uint32_t DEVICE_ID2 = DEVICE_ID1 + 0x4;
    static constexpr uint32_t DEVICE_ID3 = DEVICE_ID1 + 0x8;

private:
    static uint8_t* deviceDescriptor(USBD_SpeedTypeDef speed, uint16_t *length);
    static uint8_t* langIDStringDescriptor(USBD_SpeedTypeDef speed, uint16_t *length);
    static uint8_t* manufacturerStringDescriptor(USBD_SpeedTypeDef speed, uint16_t *length);
    static uint8_t* productStringDescriptor(USBD_SpeedTypeDef speed, uint16_t *length);
    static uint8_t* serialStringDescriptor(USBD_SpeedTypeDef speed, uint16_t *length);
    static uint8_t* configStringDescriptor(USBD_SpeedTypeDef speed, uint16_t *length);
    static uint8_t* interfaceStringDescriptor(USBD_SpeedTypeDef speed, uint16_t *length);

private:
    static void serialNumber();
    static void intToUnicode(uint32_t value, uint8_t* pbuf, uint8_t len);

private:
    bool isBusy();

    void irqHandler();
    USBD_StatusTypeDef init(USBD_HandleTypeDef*);

public:

    UsbDevice() : mHusb(), mHpcd(), mMutex(), mData() 
    {
        mData.alloc(rxSize);
    }

    virtual ~UsbDevice() {}

    bool init();

    static int8_t initFs();
    static int8_t deinitFs();
    static int8_t controlFs(uint8_t cmd, uint8_t* pbuf, uint16_t length);
    static int8_t receiveFs(uint8_t* buf, uint32_t *length);

    bool send(std::string message, std::string end = "\r\n");
    bool send(uint8_t* buf, uint8_t length);

    uint8_t readByte();
    std::string readLn(uint32_t timeout = 2000);
	bool readCount(uint8_t* data, uint32_t size, uint32_t timeout = 2000);
};
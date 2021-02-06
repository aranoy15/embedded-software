#include <usb.h>
#include <time.h>
#include <vector>

uint8_t UsbDevice::mRxBuffer[rxSize];
uint8_t UsbDevice::mTxBuffer[txSize];

USBD_CDC_ItfTypeDef UsbDevice::interfaceFs = {
    UsbDevice::initFs,
    UsbDevice::deinitFs,
    UsbDevice::controlFs,
    UsbDevice::receiveFs
};

bool UsbDevice::init()
{
	if (USBD_Init(&mHusb, &UsbDevice::fsDesc, DEVICE_FS) != USBD_OK)
		return false;

	if (USBD_RegisterClass(&mHusb, &USBD_CDC) != USBD_OK) return false;
    if (USBD_CDC_RegisterInterface(&mHusb, &interfaceFs) != USBD_OK) return false;
    if (USBD_Start(&mHusb) != USBD_OK) return false;

    return true;
}

int8_t UsbDevice::initFs()
{
	USBD_CDC_SetTxBuffer(&UsbDevice::instance()->mHusb, mTxBuffer, 0);
	USBD_CDC_SetRxBuffer(&UsbDevice::instance()->mHusb, mRxBuffer);
	return (USBD_OK);
}

int8_t UsbDevice::deinitFs()
{
    return USBD_OK;
}

int8_t UsbDevice::controlFs(uint8_t cmd, uint8_t* pbuf, uint16_t length)
{
	switch (cmd) {
		case CDC_SEND_ENCAPSULATED_COMMAND:

			break;

		case CDC_GET_ENCAPSULATED_RESPONSE:

			break;

		case CDC_SET_COMM_FEATURE:

			break;

		case CDC_GET_COMM_FEATURE:

			break;

		case CDC_CLEAR_COMM_FEATURE:

			break;

			/*******************************************************************************/
			/* Line Coding Structure */
			/*-----------------------------------------------------------------------------*/
			/* Offset | Field       | Size | Value  | Description */
			/* 0      | dwDTERate   |   4  | Number |Data terminal rate, in bits
			 * per second*/
			/* 4      | bCharFormat |   1  | Number | Stop bits */
			/*                                        0 - 1 Stop bit */
			/*                                        1 - 1.5 Stop bits */
			/*                                        2 - 2 Stop bits */
			/* 5      | bParityType |  1   | Number | Parity */
			/*                                        0 - None */
			/*                                        1 - Odd */
			/*                                        2 - Even */
			/*                                        3 - Mark */
			/*                                        4 - Space */
			/* 6      | bDataBits  |   1   | Number Data bits (5, 6, 7, 8 or
			 * 16).          */
			/*******************************************************************************/
		case CDC_SET_LINE_CODING:

			break;

		case CDC_GET_LINE_CODING:

			break;

		case CDC_SET_CONTROL_LINE_STATE:

			break;

		case CDC_SEND_BREAK:

			break;

		default:
			break;
	}

  return (USBD_OK);
}

int8_t UsbDevice::receiveFs(uint8_t* buf, uint32_t *length)
{
	for (uint32_t i = 0; i < *length; i++) {
		UsbDevice::instance()->mData.put(buf[i]);
	}

    USBD_CDC_ReceivePacket(&UsbDevice::instance()->mHusb);
    return (USBD_OK);
}

bool UsbDevice::isBusy()
{
	USBD_CDC_HandleTypeDef* hcdc =
	    reinterpret_cast<USBD_CDC_HandleTypeDef*>(mHusb.pClassData);

    if (hcdc->TxState != 0) {
        return true;
	}
    
    return false;
}

bool UsbDevice::send(std::string message, std::string end)
{
	Lock lock(mMutex);
    bool resultSendMessage = send(reinterpret_cast<uint8_t*>(message.data()), message.size());

	if (not end.empty())
		bool resultSendEndLine = send(reinterpret_cast<uint8_t*>(end.data()), end.size());
	return true;
}

bool UsbDevice::send(uint8_t* buf, uint8_t length)
{
	Lock lock(mMutex);
	if (isBusy()) return false;

    USBD_CDC_SetTxBuffer(&mHusb, buf, length);

	bool result = (USBD_CDC_TransmitPacket(&mHusb) == USBD_OK);

	Time::sleep(Time(1));

    return result;
}

uint8_t UsbDevice::readByte()
{
	if (mData.getUsedSize() > 0)
		return mData.get();	
	
	return 0;
}

std::string UsbDevice::readLn(uint32_t timeout)
{
	Lock lock(mMutex);

	std::string message;

	Timer timer;
	timer.start(timeout);
	bool resultRead = false;

	if (not mData.isEmpty()) {
		while (mData.getUsedSize() != 0 and not timer.elapsed()) {
			uint8_t data = mData.get();

			if (data == '\n') {
				resultRead = true;
				break;
			}

			message.push_back(data);
		}
	}

	if (not resultRead) message.clear();

	return message;
}

bool UsbDevice::readCount(uint8_t* data, uint32_t size, uint32_t timeout)
{
	Lock lock(mMutex);

	Timer timer;
	timer.start(timeout);

	while (not timer.elapsed()) {
		if (mData.getUsedSize() >= size) {
			for (uint32_t i = 0; i < size; i++) {
				data[i] = mData.get();
			}

			return true;
		}

		Time::sleep(Time(10));
	}

	return false;
}

USBD_StatusTypeDef UsbDevice::init(USBD_HandleTypeDef* pdev)
{
    mHpcd.pData = pdev; 

    pdev->pData = &mHpcd;

    mHpcd.Instance = USB;
    mHpcd.Init.dev_endpoints = 8;
    mHpcd.Init.speed = PCD_SPEED_FULL;
    mHpcd.Init.low_power_enable = DISABLE;
    mHpcd.Init.lpm_enable = DISABLE;
    mHpcd.Init.battery_charging_enable = DISABLE;
    HAL_PCD_Init(&mHpcd);

    HAL_PCDEx_PMAConfig((PCD_HandleTypeDef*)pdev->pData , 0x00 , PCD_SNG_BUF, 0x18);
    HAL_PCDEx_PMAConfig((PCD_HandleTypeDef*)pdev->pData , 0x80 , PCD_SNG_BUF, 0x58);
    HAL_PCDEx_PMAConfig((PCD_HandleTypeDef*)pdev->pData , 0x81 , PCD_SNG_BUF, 0xC0);
    HAL_PCDEx_PMAConfig((PCD_HandleTypeDef*)pdev->pData , 0x01 , PCD_SNG_BUF, 0x110);
    HAL_PCDEx_PMAConfig((PCD_HandleTypeDef*)pdev->pData , 0x82 , PCD_SNG_BUF, 0x100);

    return USBD_OK;
}

void UsbDevice::irqHandler()
{
    HAL_PCD_IRQHandler(&mHpcd);
}

const char* UsbDevice::USBD_MANUFACTURER_STRING = "STMicroelectronics";
const char* UsbDevice::USBD_PRODUCT_STRING_FS = "BITPP Device";
const char* UsbDevice::USBD_CONFIGURATION_STRING_FS = "CDC Config";
const char* UsbDevice::USBD_INTERFACE_STRING_FS = "CDC Interface";

USBD_DescriptorsTypeDef UsbDevice::fsDesc = {
    UsbDevice::deviceDescriptor,
    UsbDevice::langIDStringDescriptor,
    UsbDevice::manufacturerStringDescriptor,
    UsbDevice::productStringDescriptor,
    UsbDevice::serialStringDescriptor,
    UsbDevice::configStringDescriptor,
    UsbDevice::interfaceStringDescriptor};

#if defined(__ICCARM__) /* IAR Compiler */
#pragma data_alignment = 4
#endif /* defined ( __ICCARM__ ) */
/** USB standard device descriptor. */
__ALIGN_BEGIN uint8_t usbDeviceDescriptor[USB_LEN_DEV_DESC] __ALIGN_END = {
    0x12, /*bLength */
    USB_DESC_TYPE_DEVICE, /*bDescriptorType*/
    0x00, /*bcdUSB */
    0x02,
    0x02, /*bDeviceClass*/
    0x02, /*bDeviceSubClass*/
    0x00, /*bDeviceProtocol*/
    USB_MAX_EP0_SIZE, /*bMaxPacketSize*/
    LOBYTE(UsbDevice::USBD_VID), /*idVendor*/
    HIBYTE(UsbDevice::USBD_VID), /*idVendor*/
    LOBYTE(UsbDevice::USBD_PID_FS), /*idProduct*/
    HIBYTE(UsbDevice::USBD_PID_FS), /*idProduct*/
    0x00, /*bcdDevice rel. 2.00*/
    0x02,
    USBD_IDX_MFC_STR, /*Index of manufacturer  string*/
    USBD_IDX_PRODUCT_STR, /*Index of product string*/
    USBD_IDX_SERIAL_STR, /*Index of serial number string*/
    USBD_MAX_NUM_CONFIGURATION /*bNumConfigurations*/
};

#if defined(__ICCARM__) /* IAR Compiler */
#pragma data_alignment = 4
#endif /* defined ( __ICCARM__ ) */

/** USB lang indentifier descriptor. */
__ALIGN_BEGIN uint8_t usbLangIDDesc[USB_LEN_LANGID_STR_DESC] __ALIGN_END = {
    USB_LEN_LANGID_STR_DESC, USB_DESC_TYPE_STRING,
    LOBYTE(UsbDevice::USBD_LANGID_STRING),
    HIBYTE(UsbDevice::USBD_LANGID_STRING)};

#if defined(__ICCARM__) /* IAR Compiler */
#pragma data_alignment = 4
#endif /* defined ( __ICCARM__ ) */
/* Internal string descriptor. */
__ALIGN_BEGIN uint8_t usbStrDesc[USBD_MAX_STR_DESC_SIZ] __ALIGN_END;

#if defined(__ICCARM__) /*!< IAR Compiler */
#pragma data_alignment = 4
#endif
__ALIGN_BEGIN uint8_t
    USBD_StringSerial[UsbDevice::USB_SIZ_STRING_SERIAL] __ALIGN_END = {
        UsbDevice::USB_SIZ_STRING_SERIAL,
        USB_DESC_TYPE_STRING,
};


uint8_t *UsbDevice::deviceDescriptor(USBD_SpeedTypeDef speed,
                                         uint16_t *length)
{
	(void)speed;
	*length = sizeof(usbDeviceDescriptor);
	return usbDeviceDescriptor;
}

uint8_t *UsbDevice::langIDStringDescriptor(USBD_SpeedTypeDef speed,
                                               uint16_t *length)
{
	(void)speed;
	*length = sizeof(usbLangIDDesc);
	return usbLangIDDesc;
}

uint8_t *UsbDevice::productStringDescriptor(USBD_SpeedTypeDef speed,
                                                uint16_t *length)
{
	if (speed == 0) {
		USBD_GetString((uint8_t *)USBD_PRODUCT_STRING_FS, usbStrDesc, length);
	} else {
		USBD_GetString((uint8_t *)USBD_PRODUCT_STRING_FS, usbStrDesc, length);
	}
	return usbStrDesc;
}

uint8_t *UsbDevice::manufacturerStringDescriptor(USBD_SpeedTypeDef speed,
                                                     uint16_t *length)
{
	(void)speed;
	USBD_GetString((uint8_t *)USBD_MANUFACTURER_STRING, usbStrDesc, length);
	return usbStrDesc;
}

uint8_t *UsbDevice::serialStringDescriptor(USBD_SpeedTypeDef speed,
                                               uint16_t *length)
{
	(void)speed;
	*length = USB_SIZ_STRING_SERIAL;
	serialNumber();

	return (uint8_t *)USBD_StringSerial;
}

uint8_t *UsbDevice::configStringDescriptor(USBD_SpeedTypeDef speed,
                                                    uint16_t *length)
{
	if (speed == USBD_SPEED_HIGH) {
		USBD_GetString((uint8_t *)USBD_CONFIGURATION_STRING_FS, usbStrDesc,
		               length);
	} else {
		USBD_GetString((uint8_t *)USBD_CONFIGURATION_STRING_FS, usbStrDesc,
		               length);
	}
	return usbStrDesc;
}

uint8_t *UsbDevice::interfaceStringDescriptor(USBD_SpeedTypeDef speed,
                                                       uint16_t *length)
{
	if (speed == 0) {
		USBD_GetString((uint8_t *)USBD_INTERFACE_STRING_FS, usbStrDesc,
		               length);
	} else {
		USBD_GetString((uint8_t *)USBD_INTERFACE_STRING_FS, usbStrDesc,
		               length);
	}
	return usbStrDesc;
}

void UsbDevice::serialNumber()
{
	uint32_t deviceserial0, deviceserial1, deviceserial2;

	deviceserial0 = *(uint32_t *)DEVICE_ID1;
	deviceserial1 = *(uint32_t *)DEVICE_ID2;
	deviceserial2 = *(uint32_t *)DEVICE_ID3;

	deviceserial0 += deviceserial2;

	if (deviceserial0 != 0) {
		intToUnicode(deviceserial0, &USBD_StringSerial[2], 8);
		intToUnicode(deviceserial1, &USBD_StringSerial[18], 4);
	}
}

void UsbDevice::intToUnicode(uint32_t value, uint8_t *pbuf, uint8_t len)
{
	uint8_t idx = 0;

	for (idx = 0; idx < len; idx++) {
		if (((value >> 28)) < 0xA) {
			pbuf[2 * idx] = (value >> 28) + '0';
		} else {
			pbuf[2 * idx] = (value >> 28) + 'A' - 10;
		}

		value = value << 4;

		pbuf[2 * idx + 1] = 0;
	}
}
#pragma once

#include <usb.h>
#include <flash.h>
#include <string>
#include <taskscheduler.h>

class CommandHandler : public TaskBase
{

private:
	UsbDevice& usb() { return *UsbDevice::instance(); }

	const uint32_t headerAddress = 0;
	const uint32_t firmwareAddress = 1024;
	const uint32_t packetSize = 1024;

private:
	void doUpload();
	void doDateTime(std::string& data);

	int16_t crc16(const uint8_t* data, uint32_t size);
	bool checkPacket(uint32_t address, uint8_t* data, uint32_t size);

public:
	CommandHandler() : TaskBase(10) {}
    virtual ~CommandHandler() {}

	void setup() override;
	void func() override;
};
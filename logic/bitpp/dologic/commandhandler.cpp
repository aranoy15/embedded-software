#include <commandhandler.h>
//#include <datetimemanager.h>
//#include <datetime.h>

namespace
{
__attribute__((constructor)) void reg()
{
	TaskScheduler::instance()->reg(new CommandHandler());
}
}  // namespace

void CommandHandler::setup()
{

}

void CommandHandler::func()
{
    auto line = usb().readLn(10);

    if (not line.empty()) {
        if (line == "AT+REBOOT") 
            doUpload();
        else if (line.rfind("AT+SETDATETIME ", 0) == 0) {
            line.erase(0, 15);
            doDateTime(line);
        }
    }
}

void CommandHandler::doDateTime(std::string& data)
{
    //DateTime dt(data.c_str());
    //DateTimeManager::instance()->set(dt);
}

void CommandHandler::doUpload()
{
    usb().send(">", "");

    Time::sleep(Time(10));

    if (usb().readByte() == '<') {
        uint16_t countPackets = 0;
        uint8_t secretByte = 0x87;

        usb().send("!", "");

        uint16_t packetNumber = 0;
        uint16_t packetCrc = 0;
        uint8_t packet[packetSize];

        Timer timer(Time::minute());

        timer.start();

        Flash::instance()->erase4k(headerAddress);

        while (not timer.elapsed()) {
			if (not usb().readCount(reinterpret_cast<uint8_t*>(&packetNumber),
			                        sizeof(packetNumber)))
				return;
            
            if (packetNumber == 0xFFFF) break;

			if (not usb().readCount(packet, sizeof(packet))) return;
			if (not usb().readCount(reinterpret_cast<uint8_t*>(&packetCrc),
			                        sizeof(packetCrc)))
				return;

			uint16_t crc = crc16(packet, 1024);

            if (crc != packetCrc) return;

            uint32_t addressToFlash = (countPackets * packetSize) + firmwareAddress;

            if ((addressToFlash % 4096) == 0)
                Flash::instance()->erase4k(addressToFlash);

            Flash::instance()->writeData(addressToFlash, packet, sizeof(packet));

			if (not checkPacket(addressToFlash, packet, sizeof(packet))) return;

            countPackets += 1;
			usb().send("!", ""); 
		}

        uint8_t header[sizeof(secretByte) + sizeof(countPackets)];

        header[0] = secretByte;
        header[1] = countPackets;
        header[2] = countPackets >> 8;

        Flash::instance()->writeData(headerAddress, header, sizeof(header));

        if (not checkPacket(headerAddress, header, sizeof(header))) return;

        usb().send("!", "");
    }
}

bool CommandHandler::checkPacket(uint32_t address, uint8_t* data, uint32_t size)
{
    if (size == 0) return false;

    bool result = false;

    uint8_t readData[size];

    for (uint8_t i = 0; i < 3; i++) {
        Flash::instance()->read(address, readData, size);
        bool resultCmp = (memcmp(readData, data, size) == 0);

        if (resultCmp) {
            result = true;
            break;
        }

        Time::sleep(Time(10));
    }

    return result;
}

int16_t CommandHandler::crc16(const uint8_t* data, uint32_t size)
{
	uint16_t crc = 0;
	for (uint32_t i = 0; i < size; ++i) {
		crc ^= data[i];

		bool bitMoved = (crc & 1 << 15) != 0;
		crc <<= 1;

		if (bitMoved) crc |= 1;
	}

	return crc;
}
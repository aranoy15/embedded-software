#include <library/packetmanager.h>
#include <library/utils.h>

PacketManager::PacketManager(quint32 packetSize, QObject *parent)
    : QObject(parent), mPacketSize(packetSize)//, mData()
{
}

void PacketManager::loadData(const QByteArray& data)
{
    //mData = data;

    mHeader.clear(); 
    mPackets.clear();

    quint16 packetSizeValue = packetSize();
    quint16 packetCountValue = (data.size() / packetSizeValue) + 1;

    mHeader.push_back(0x76);
    mHeader.push_back(packetCountValue >> 8);
    mHeader.push_back(packetCountValue & 0xFF);
    mHeader.push_back(packetSizeValue >> 8);
    mHeader.push_back(packetSizeValue & 0xFF);

    quint16 crc = utils::crc16(mHeader);

    mHeader.push_back(crc & 0xFF);
    mHeader.push_back(crc >> 8);

    for (quint16 i = 0; i < packetCountValue; i++) {
        QByteArray packet;

        qint32 start = packetSizeValue * i;
        qint32 end = start + packetSizeValue;

        if (end > data.size())
            end = data.size();

        for (auto iter = (data.begin() + start); iter != (data.begin() + end);
	     ++iter) {
		    packet.push_back(*iter);
	    }

	    if (packet.size() < packetSizeValue) {
            int completeLength = packet.size();

            for (int i = 0; i < (packetSizeValue - completeLength); i++) {
                packet.push_back(char(0xFF));
            }
        }
        
        quint16 crc = utils::crc16(packet);

        packet.push_back(crc & 0xFF);
        packet.push_back(crc >> 8);

        mPackets.push_back(packet);
    }
}

const QByteArray& PacketManager::getHeader()
{
    return mHeader; 
}

const QByteArray& PacketManager::getPacket(quint16 index)
{
    return mPackets[index];
}

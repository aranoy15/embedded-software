#pragma once

#include <QObject>
#include <QByteArray>
#include <QList>

class PacketManager : public QObject
{
    Q_OBJECT

    quint32 mPacketSize;
    //QByteArray mData;

    QByteArray mHeader;
    QList<QByteArray> mPackets; 

public:

    PacketManager(quint32, QObject * = nullptr);
    virtual ~PacketManager() {}

	quint16 packetSize() const { return mPacketSize; }
	quint16 packetCount() const { return mPackets.size(); }//(mData.length() / packetSize() + 1); }

	//const QByteArray& data() const { return mData; }
	//int countPackets() const { return (mData.length() / mPacketSize) + 1; }

	void loadData(const QByteArray& data);
    const QByteArray& getHeader();
    const QByteArray& getPacket(quint16);
};
#include <library/reader.h>
#include <iostream>
#include <QThread>
#include <QDebug>

Reader::Reader(ReaderType type, QObject* parent)
    : QObject(parent), mSerial(), mTempSerial(), mLines(), mType(type)
{
	mSerial.setBaudRate(QSerialPort::Baud115200);
	mSerial.setDataBits(QSerialPort::Data8);
	mSerial.setParity(QSerialPort::NoParity);
	mSerial.setStopBits(QSerialPort::OneStop);
	mSerial.setFlowControl(QSerialPort::NoFlowControl);
	mSerial.setReadBufferSize(4096);

	connect(&mSerial, &QSerialPort::readyRead, this, &Reader::readyRead);
}

bool Reader::open(const QString& portName)
{
    mSerial.setPortName(portName);

    if (mSerial.open(QIODevice::ReadWrite)) {
        if (mSerial.isOpen()) {
            mSerial.clear(QSerialPort::AllDirections);
            return true;
        }
    }

    return false;
}

QByteArray Reader::read()
{
    QByteArray result;

	if (mType == ReaderType::Raw and not mTempSerial.isEmpty()) {
		result = mTempSerial;
		mTempSerial.clear();
	}

	return result;
}


QList<QByteArray> Reader::readLines()
{
    QList<QByteArray> result;

	if (mType == ReaderType::Message) {
		for (auto& item : mLines)
			result.append(item);

		mLines.clear();
	}

	return result;
}


void Reader::readyRead()
{
	if (mType == ReaderType::Message) {
		QByteArray tempLine;

		if (not mTempSerial.isEmpty()) {
			tempLine += mTempSerial;
			mTempSerial.clear();
		}

		mSerial.waitForBytesWritten();

		auto bytes = mSerial.readAll();

		for (auto ch : bytes) {
			tempLine += ch;

			if (ch == '\n') {
				mLines.push_back(tempLine);
				tempLine.clear();
			}
		}

		if (not tempLine.isEmpty()) mTempSerial = tempLine;
	} else if (mType == ReaderType::Raw) {
		auto res = mSerial.readAll();
        mTempSerial.append(res);
    }
}

void Reader::writeRaw(const QByteArray& data)
{
    mSerial.write(data);
}

void Reader::writeLine(const QByteArray& data)
{
    mSerial.write(data);

    if (not data.contains("\n"))
        mSerial.write("\n");
}

void Reader::reload()
{
    mSerial.setDataTerminalReady(true);
    QThread::msleep(2);
    mSerial.setDataTerminalReady(false);
}
#pragma once

#include <QSerialPort>
#include <QSerialPortInfo>

enum class ReaderType
{
    Message = 1,
    Raw = 2
};

class Reader : public QObject
{
    Q_OBJECT

private:

    QSerialPort mSerial;
    QByteArray mTempSerial;
    QList<QByteArray> mLines;
    ReaderType mType;

public slots:
    void readyRead(); 
    void reload();

public:

    Reader(const Reader&) = delete;
	Reader operator=(const Reader&) = delete;

	explicit Reader(ReaderType, QObject* = nullptr);
    virtual ~Reader() {}

    bool open(const QString& portName);

    void write(QByteArray& data);

    QByteArray read();
    QList<QByteArray> readLines();

	QList<QSerialPortInfo> ports() { return QSerialPortInfo::availablePorts(); }
	void setBaud(int baud) { mSerial.setBaudRate(baud); }

public slots:
    void writeRaw(const QByteArray&);
    void writeLine(const QByteArray&);
};
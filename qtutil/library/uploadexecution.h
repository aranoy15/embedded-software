#pragma once

#include <QObject>
#include <QTime>
#include <memory>
#include <library/reader.h>
#include <library/packetmanager.h>
#include <QElapsedTimer>
#include <QThread>

class UploadExecution : public QThread
{
    Q_OBJECT

private:
    QTime mTime;
    std::unique_ptr<Reader> mReader;
    std::unique_ptr<PacketManager> mPacketManager;

private:
    bool loadPackets(const QString&);
    bool waitData(const QByteArray&);

signals:
    void reloadDevice();
    void write(const QByteArray&);
    void testFinished(bool);
    void updateStageSignal(int);

public slots:
    void uploadProcess();
    void exitFromApplication(bool);

private slots:
    void updateStage(int);

public:
	UploadExecution(const UploadExecution&) = delete;
	UploadExecution operator=(const UploadExecution&) = delete;

	explicit UploadExecution(QObject* = nullptr);
    virtual ~UploadExecution() {}

	void startExec(const QString& port, const QString& filePath, int baud = 115200);
    void run() override;
};
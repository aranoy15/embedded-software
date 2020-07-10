#include <QCoreApplication>
#include <QDebug>
#include <QFileInfo>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <library/uploadexecution.h>
#include <unistd.h>

#ifdef Q_OS_UNIX
#include <sys/ioctl.h>
struct winsize w;
#endif

using namespace std;

UploadExecution::UploadExecution(QObject* parent)
    : QThread(parent),
      mTime(),
      mReader(new Reader(ReaderType::Raw)),
      mPacketManager(new PacketManager(1024))
{
	connect(this, &UploadExecution::reloadDevice, mReader.get(),
	        &Reader::reload);
	connect(this, &UploadExecution::write, mReader.get(), &Reader::writeRaw);
	connect(this, &UploadExecution::testFinished, this,
	        &UploadExecution::exitFromApplication);
	connect(this, &UploadExecution::updateStageSignal, this,
	        &UploadExecution::updateStage);

#ifdef Q_OS_UNIX
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
#endif
}

void UploadExecution::exitFromApplication(bool)
{
	QCoreApplication::quit();
}

bool UploadExecution::waitData(const QByteArray& waitData)
{
	QElapsedTimer timer;
	timer.start();

	while (not timer.hasExpired(60000)) {
		auto data = mReader->read();

		if (not data.isEmpty()) {
			//qDebug() << "Read data: " << data;
			//qDebug() << "Wait data: " << waitData;

			if (data.contains(waitData)) return true;
		}

		QThread::msleep(10);
	}

	return false;
}

bool UploadExecution::loadPackets(const QString& filePath)
{
	auto fileInfo = QFileInfo(filePath);

	if (not fileInfo.exists()) return false;
	if (not fileInfo.isFile()) return false;

	QFile file(fileInfo.filePath());

	if (not file.open(QIODevice::ReadOnly)) return false;

	mPacketManager->loadData(file.readAll());

	file.close();
	return true;
}

void UploadExecution::startExec(const QString& port, const QString& filePath,
                                int baud)
{
	mReader->setBaud(baud);

	if (not loadPackets(filePath)) {
		qInfo() << "Error load packets";
		return;
	}

	if (not mReader->open(port)) {
		qInfo() << "Error open serial port";
		return;
	}

	start();
}


void UploadExecution::run()
{
	uploadProcess();
}

void UploadExecution::uploadProcess()
{
	// for (int i = 0; i < mPacketManager->packetCount(); i++) {
	//    emit updateStageSignal(i + 1);
	//    msleep(100);
	//}

	cout << "start upload" << endl << flush;

	emit reloadDevice();

	msleep(10);

	if (not waitData(">")) {
		qDebug() << "not start upload";
		return;
	}

	msleep(100);

	emit write("<");

	if (not waitData("!")) {
		qDebug() << "not erase app";
		return;
	}

	mTime.start();

	for (uint16_t i = 0; i < mPacketManager->packetCount(); i++) {

		QByteArray packetNumber;

		packetNumber.append(static_cast<char>(i));
		packetNumber.append(static_cast<char>(i >> 8));

		emit write(packetNumber);

		emit write(mPacketManager->getPacket(i));

		if (not waitData("!")) {
			qDebug() << "Not upload data: " << i;
			return;
		}

		emit updateStageSignal(i + 1);
	}

	QByteArray end;

	end.append(0xFF);
	end.append(0xFF);

	emit write(end);

	if (not waitData("!")) {
		qDebug() << "Not upload complete data";
		return;
	}

	emit testFinished(true);
	cout << endl << flush;
}

void UploadExecution::updateStage(int stage)
{
	auto terminalWidth = w.ws_col;

	auto columnForProgress = terminalWidth - 70;

	float percent =
	    (100.0f * stage) / static_cast<double>(mPacketManager->packetCount());
	cout << "\r" << fixed << setprecision(1) << setw(8) << percent << "%"
	     << flush;
	cout << setw(2) << setfill(' ') << "[" << flush;

	int countToFill = (columnForProgress * percent) / 100;

	for (int i = 0; i < columnForProgress; i++) {
		if (i < countToFill)
			cout << "=" << flush;
		else if (i == countToFill)
			cout << ">" << flush;
		else
			cout << " " << flush;
	}

	cout << "] ";

	float secondsRemains = mTime.elapsed() / 1000.0f;
	float countKbSended = (mPacketManager->packetSize() * stage) / 1024.0f;
	float speed = countKbSended / secondsRemains;

	cout << fixed << setprecision(1) << setw(11) << speed << "kB/s" << flush;

	float totalSize =
	    ((mPacketManager->packetSize() * mPacketManager->packetCount()) /
	     1024.0f) -
	    countKbSended;
	// qDebug() << "\nTotal size: " << totalSize;
	int timeToFinish = ceil(totalSize / speed);

	cout << setw(6) << timeToFinish << "s" << flush;
}
#include <QCoreApplication>
#include <QCommandLineParser>
#include <QDebug>

#include <library/uploadexecution.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QCoreApplication::setApplicationName("reader");
    QCoreApplication::setApplicationVersion("1.0");

    QCommandLineParser parser;
    parser.setApplicationDescription("Application for read message from device by uart");
    parser.addHelpOption();
    parser.addVersionOption();

	QCommandLineOption comPortOption(
	    "c", QCoreApplication::translate("main", "Open serial port by <name> (required)"),
	    QCoreApplication::translate("main", "name"));

	parser.addOption(comPortOption);

    QCommandLineOption firmwarePathOption(
	    "f", QCoreApplication::translate("main", "Choose <firmware> to upload (required)"),
	    QCoreApplication::translate("main", "firmware"));

	parser.addOption(firmwarePathOption);

	QCommandLineOption baudRateOption(
	    "b", QCoreApplication::translate("main", "Baudrate of serial port"),
	    QCoreApplication::translate("main", "baudrate"), "115200");

    parser.addOption(baudRateOption);   

    parser.process(a);

    auto baudRate = parser.value(baudRateOption).toInt();
    auto port = parser.value(comPortOption);
    auto firmware = parser.value(firmwarePathOption);

    if (port.isEmpty() or not parser.isSet(comPortOption)) {
        qCritical() << "-- Error: port is not set --";
        parser.showHelp();
        return 1;
    }

    if (firmware.isEmpty() or not parser.isSet(firmwarePathOption)) {
        qCritical() << "-- Error: firmware is not choose";
        parser.showHelp();
        return 1;
    }

	UploadExecution ue;
    ue.startExec(port, firmware, baudRate);
    //ue.start();

	return a.exec();
}
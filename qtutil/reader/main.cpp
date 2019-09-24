#include <QCoreApplication>
#include <QCommandLineParser>
#include <QDebug>
#include <QLocale>

#include <library/readerexecution.h>

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

	QCommandLineOption baudRateOption(
	    "b", QCoreApplication::translate("main", "Baudrate of serial port"),
	    QCoreApplication::translate("main", "baudrate"), "115200");

    parser.addOption(baudRateOption);   

    parser.process(a);

    auto baudRate = parser.value(baudRateOption).toInt();
    auto port = parser.value(comPortOption);

    if (port.isEmpty() or not parser.isSet(comPortOption)) {
        qCritical() << "-- Error: port is not set --";
        parser.showHelp();
        return 1;
    }

	ReaderExecution re;
    re.start(port, baudRate);

	return a.exec();
}
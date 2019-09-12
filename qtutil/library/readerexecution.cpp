#include <library/readerexecution.h>

#include <stdio.h>
#include <unistd.h>
#include <QDateTime>
#include <iostream>

#ifdef Q_OS_UNIX
#include <sys/ioctl.h>
struct winsize w;
#endif

#ifdef Q_OS_WIN
#include <windows.h>
#endif

using namespace std;

ReaderExecution::ReaderExecution(QObject* parent)
    : QObject(parent),
      mTimer(),
      mReader(new Reader(ReaderType::Message)),
      mCharCatcher(new CharCatcher(*mReader.get()))
{
	#ifdef Q_OS_UNIX
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	#endif

	connect(&mTimer, &QTimer::timeout, this, &ReaderExecution::terminalProcess);
}

void ReaderExecution::start(QString& port, int baud)
{
	mReader->setBaud(baud);
    bool result = mReader->open(port);

	clearLine();
	cout << formatLine(QString("result open: %0").arg(result).toLocal8Bit()).toStdString() << endl << flush;

	mTimer.start(10);
    mCharCatcher->start();

}

QString ReaderExecution::formatLine(const QByteArray& message)
{
	return QString("[%1] %0").arg(
	    message, QDateTime::currentDateTime().toString("HH:mm:ss.zzz"));
}

void ReaderExecution::terminalProcess()
{
    processLine(); 
    processCommand();
}

void ReaderExecution::clearLine()
{
	int columns = 0;

	#ifdef Q_OS_UNIX
		columns = w.ws_col;
	#endif

	#ifdef Q_OS_WIN
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
		columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	#endif

	std::cout << "\r" << std::flush;
	for (int i = 0; i < columns; i++)
		std::cout << " " << std::flush;
	std::cout << "\r" << std::flush;
}

void ReaderExecution::processLine()
{
	auto data = mReader->readLines();

	for (auto line : data) {
		clearLine();
		cout << formatLine(line).toStdString() << flush;
	}
}

void ReaderExecution::processCommand()
{
    clearLine();
    cout << " > " << mCharCatcher->message().toStdString();

    for (uint64_t i = 0; i < mCharCatcher->cursorPosition(); i++)
        cout << "\x1B[D";
    
    cout << flush;
}
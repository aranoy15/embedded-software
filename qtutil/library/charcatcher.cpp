#include <library/charcatcher.h>
//#include <curses.h>
#include <QDebug>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QCoreApplication>
#include <QDateTime>
#include <QByteArray>

#ifdef Q_WS_WIN
//#include <termiwin.h>
#else
//#include <termios.h>
#endif

#include <iostream>

using namespace std;

/*
static struct termios oldSettings;
static struct termios newSettings;

void initTermios(int echo) 
{
  tcgetattr(0, &oldSettings); 
  newSettings = oldSettings; 
  newSettings.c_lflag &= ~ICANON; 
  newSettings.c_lflag &= echo ? ECHO : ~ECHO; 
  tcsetattr(0, TCSANOW, &newSettings);
}

void resetTermios(void) 
{
  tcsetattr(0, TCSANOW, &oldSettings);
}
*/

CharCatcher::CharCatcher(Reader& reader)
    : QThread(nullptr),
      mHistory(),
      mHistoryPosition(0),
      mCursorPosition(0),
      mMessage(),
      mKey(),
      mReader(reader),
      mKeyRegex(new QRegularExpression("\x1B\\[\\d")),
      mNeedStop(false)
{
	//initTermios(0);

    connect(this, &CharCatcher::writeLine, &mReader, &Reader::writeLine);
    connect(this, &CharCatcher::reloadDevice, &mReader, &Reader::reload);

    system("stty raw");
}

CharCatcher::~CharCatcher()
{
    //resetTermios();
    system("stty cooked");
}

void CharCatcher::run()
{
    mMessage.clear();


    while(not mNeedStop)
    {
        processCommand();
    }
}

void CharCatcher::processCommand()
{

	char key = getchar();

	QByteArray keyBytes;

	keyBytes.push_back(key);

	if (key == 0x1B) {
		keyBytes.push_back(getchar());
		keyBytes.push_back(getchar());

		if (mKeyRegex->match(keyBytes).hasMatch()) {
			keyBytes.push_back(getchar());
		}

		if (mSpecialKeys.contains(keyBytes)) mSpecialKeys[keyBytes]();

	} else if (mSpecialKeys.contains(keyBytes)) {
		mSpecialKeys[keyBytes]();
	} else {
        mMessage.insert(mMessage.size() - mCursorPosition, keyBytes);
    }

    //system("stty cooked");
}

void CharCatcher::doExit()
{
    //std::cout << "do exit" << endl << flush;
    mNeedStop = true;
    QCoreApplication::quit();
}

void CharCatcher::doBackword()
{
    int countRemoved = 0;
    for (int i = (mMessage.size() - mCursorPosition - 1); i >= 0; i--) {
		if (mMessage[i] == ' ' and countRemoved != 0) {
			break;
		} else {
			mMessage.remove(i, 1);
			countRemoved++;
		}
	}
}

void CharCatcher::doBackspace()
{
    auto position = mMessage.size() - mCursorPosition - 1;
    mMessage.remove(position, 1);
}

void CharCatcher::doDelete()
{
    if (mCursorPosition == 0)
        return;
    
    int index = mMessage.size() - mCursorPosition;

    if (index >= 0)
        mMessage.remove(index, 1);
}

void CharCatcher::doEnter()
{
    //std::cout << "do enter" << endl << flush;

    if (mMessage == "!dtr")
        emit reloadDevice();
    else if (mMessage == "!sdt") {
        QLocale::setDefault(QLocale::English);
		auto dateTimeString = QLocale().toString(QDateTime::currentDateTime(),
		                                         "MMM dd yyyy hh:mm::ss");
		QByteArray dateTimeMessage; 
        dateTimeMessage.append("AT+SETDATETIME ");
        dateTimeMessage.append(dateTimeString);

        emit writeLine(dateTimeMessage);
    } else
        emit writeLine(mMessage);

    mMessage.clear();
    mCursorPosition = 0;
}

void CharCatcher::doDown()
{
    std::cout << "do down" << endl << flush;
}

void CharCatcher::doUp()
{
    std::cout << "do up" << endl << flush;
}

void CharCatcher::doRight()
{
    if (mCursorPosition > 0)
        mCursorPosition--;
}

void CharCatcher::doLeft()
{
    if (mCursorPosition < mMessage.size())
        mCursorPosition++;
}

void CharCatcher::doHome()
{
    mCursorPosition = mMessage.size();
}

void CharCatcher::doEnd()
{
    mCursorPosition = 0;
}

void CharCatcher::doComplete()
{
    std::cout << "do complete" << endl << flush;
}
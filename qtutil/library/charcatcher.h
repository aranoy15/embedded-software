#pragma once

#include <QThread>
#include <QMap>
#include <functional>
#include <library/reader.h>

class CharCatcher : public QThread
{
    Q_OBJECT

public:
    using SpecialKeysType = QMap<QByteArray, std::function<void(void)>>;

signals:
    void write(const QByteArray&);
    void writeLine(const QByteArray&);
    void reloadDevice();

private:

    QList<QByteArray> mHistory;
    int mHistoryPosition;
    int mCursorPosition;
    QByteArray mMessage;
    QByteArray mKey;
    Reader& mReader;
    std::unique_ptr<QRegularExpression> mKeyRegex;
    bool mNeedStop;


    const SpecialKeysType mSpecialKeys = {
        {"\x03", std::bind(&CharCatcher::doExit, this)},
        {"\x04", std::bind(&CharCatcher::doExit, this)},
        {"\x17", std::bind(&CharCatcher::doBackword, this)},
        {"\x7F", std::bind(&CharCatcher::doBackspace, this)},
        {"\x08", std::bind(&CharCatcher::doBackspace, this)},
        {"\x1B[3~", std::bind(&CharCatcher::doDelete, this)},
        {"\x0D", std::bind(&CharCatcher::doEnter, this)},
        {"\x0A", std::bind(&CharCatcher::doDown, this)},
        {"\x0B", std::bind(&CharCatcher::doUp, this)},
        {"\x1B[A", std::bind(&CharCatcher::doUp, this)},
        {"\x1B[B", std::bind(&CharCatcher::doDown, this)},
        {"\x1B[C", std::bind(&CharCatcher::doRight, this)},
        {"\x1B[D", std::bind(&CharCatcher::doLeft, this)},
        {"\x01", std::bind(&CharCatcher::doHome, this)},
        {"\x05", std::bind(&CharCatcher::doEnd, this)},
        {"\x1B[1~", std::bind(&CharCatcher::doHome, this)},
        {"\x1B[4~", std::bind(&CharCatcher::doEnd, this)},
        {"\t", std::bind(&CharCatcher::doComplete, this)}
    };

private:
    void processCommand();

    void doExit();
    void doBackword();
    void doBackspace();
    void doDelete();
    void doEnter();
    void doDown();
    void doUp();
    void doRight();
    void doLeft();
    void doHome();
    void doEnd();
    void doComplete();

public:
    CharCatcher(Reader& reader);
    virtual ~CharCatcher();

    void run() override;

	const QByteArray& message() const { return mMessage; }
	quint64 cursorPosition() const { return mCursorPosition; }
};
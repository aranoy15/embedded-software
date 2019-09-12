#pragma once

#include <QObject>
#include <QTimer>
#include <memory>
#include <library/reader.h>
#include <library/charcatcher.h>


class ReaderExecution : public QObject
{
    Q_OBJECT

private:
    QTimer mTimer;
    std::unique_ptr<Reader> mReader;
    std::unique_ptr<CharCatcher> mCharCatcher;

private:
    QString formatLine(const QByteArray& message);
    void clearLine();
    void processLine();
    void processCommand();

public slots:
    void terminalProcess();

public:
	ReaderExecution(const ReaderExecution&) = delete;
	ReaderExecution operator=(const ReaderExecution&) = delete;

	explicit ReaderExecution(QObject* = nullptr);
    virtual ~ReaderExecution() {}

    void start(QString& port, int baud = 115200);
};
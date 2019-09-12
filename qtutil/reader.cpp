#include <QCoreApplication>

#include <library/readerexecution.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QCoreApplication::setApplicationName("reader");
    QCoreApplication::setApplicationVersion("1.0");
    
    ReaderExecution re;
    re.start();

	return a.exec();
}
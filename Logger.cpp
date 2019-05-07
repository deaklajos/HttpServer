#include "Logger.h"

#include <QDateTime>
#include <QFile>
#include <QMutex>
#include <QTextStream>
#include <QDebug>
#include <QThread>

void Logger::Log(QtMsgType type, const QString &message)
{
    static QMutex mutex;

    QString threadString = QString::number((long long)QThread::currentThreadId());
    QDateTime dateTime(QDateTime::currentDateTime());
    QString timeString(dateTime.toString("dd-MM-yyyy HH:mm:ss:zzz"));
    QString typeString;
    switch (type)
    {
    case QtInfoMsg:
        typeString = "[INFO]";
        break;
    case QtDebugMsg:
        typeString = "[DEBUG]";
        break;
    case QtWarningMsg:
        typeString = "[WARNING]";
        break;
    case QtCriticalMsg:
        typeString = "[CRITICAL]";
        break;
    case QtFatalMsg:
        typeString = "[FATAL]";
        break;
    }
    QString entryString("%1 [%2] %3: %4");
    entryString = entryString.arg(typeString).arg(threadString).arg(timeString).arg(message);

    mutex.lock();

    static QFile outFile("log.log");
    static QTextStream stream(&outFile);
    if(!outFile.isOpen())
        outFile.open(QIODevice::WriteOnly | QIODevice::Append);
    stream << entryString << endl;
    qDebug() << entryString;

    mutex.unlock();
}

#include "Logger.h"

#include <QDateTime>
#include <QFile>
#include <QDebug>
#include <QThread>
#include <QMutexLocker>
#include <mutex>

QMutex Logger::mutex;

void Logger::Log(QtMsgType type, const QString &message)
{
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

    static QFile outFile(this->logLocation + "log.log");
    static QTextStream stream(&outFile);
    static std::once_flag flag;

    {
        QMutexLocker locker(&(this->mutex));
        std::call_once(flag, [](){ outFile.open(QIODevice::WriteOnly | QIODevice::Append); });
        stream << entryString << endl;
        qDebug() << entryString;
    }
}

void Logger::SetLogLocation(const QString& logLocation)
{
    QMutexLocker locker(&(this->mutex));
    this->logLocation = logLocation;
}

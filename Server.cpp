#include <QNetworkInterface>
#include <QCoreApplication>
#include <QProcess>
#include <unistd.h>

#include "Server.h"
#include "ResponseWorker.h"
#include "Logger.h"
#include "HttpResponse.h"

Server::Server(QObject *parent) :
    QTcpServer(parent)
{
    pool = new QThreadPool(this);
    // TODO use parameter
    pool->setMaxThreadCount(5);

    timer.start();
}

int Server::startServer()
{
    if(checkPHPInstalled()) return -1;

    auto ruid = getuid();
    auto euid = geteuid();
    auto rgid = getgid();
    auto egid = getegid();

    if(ruid == 0 || rgid == 0)
    {
        Logger::getInstance().Log(
                    QtMsgType::QtFatalMsg,
                    QString("Do not run this app as root!"));
        return -1;
    }
    if(euid != 0 || egid != 0)
    {
        Logger::getInstance().Log(
                    QtMsgType::QtFatalMsg,
                    QString("Install with \"make install\" that way permissions will be set up properly!"));
        return -1;
    }

    const qint16 port = 80;
    if(this->listen(QHostAddress::Any, port))
    {
        int userNotDropped = seteuid(ruid);
        int groupNotDropped = setegid(rgid);
        if(userNotDropped || groupNotDropped)
        {
            Logger::getInstance().Log(QtMsgType::QtFatalMsg, "Privileges could not be dropped, terminating!");
            return -1;
        }
        auto xeuid = geteuid();
        auto xegid = getegid();
        Logger::getInstance().Log(QtMsgType::QtInfoMsg,
                                  QString("Privileges dropped, euid = %1, egid = %2").arg(xeuid).arg(xegid));

        QString addressString;
        const QHostAddress &localhost = QHostAddress(QHostAddress::LocalHost);
        // TODO check if no address, or widen the printed range.
        for (const QHostAddress &address: QNetworkInterface::allAddresses()) {
            if (address.protocol() == QAbstractSocket::IPv4Protocol && address != localhost)
                 addressString += (", " + address.toString() + ":" + QString::number(port));
        }

        addressString.remove(0, 2);
        Logger::getInstance().Log(QtMsgType::QtInfoMsg, "Server started started " + addressString);
    }
    else
    {
        Logger::getInstance().Log(QtMsgType::QtFatalMsg, "Server could not be started. Reason : " + this->errorString());
        return -1;
    }

    return 0;
}

void Server::incomingConnection(qintptr handle)
{
    connectionCounter++;
    qint64 time = timer.elapsed();

    if(time > 1000)
    {
        connectionCounter = 0;
        timer.restart();
    }
    else
    {
        // TODO use parameter
        if(connectionCounter > 100)
        {
            QTcpSocket socket;
            socket.setSocketDescriptor(handle);
            // Without wait TCP RST is sent
            // TODO Add timeout and handle in thread
            socket.waitForReadyRead();
            // TODO refuse this elsewhere?
            socket.write(HttpResponse::serviceUnavailabe(true).getByteArray());
            socket.flush();
            socket.waitForBytesWritten();
            socket.close();
            Logger::getInstance().Log(QtMsgType::QtWarningMsg, "Server overloaded, connection refused.");
            return;
        }
    }

    ResponseWorker *task = new ResponseWorker();
    task->setAutoDelete(true);

    task->socketDescriptor = handle;

    pool->start(task);
}

int Server::checkPHPInstalled() const
{
    QProcess process;
    process.setProcessChannelMode(QProcess::MergedChannels);
    process.start("php", QStringList() << "-i");
    process.waitForStarted();
    process.waitForFinished();

    if(process.exitCode() != 0)
    {
        Logger::getInstance().Log(
                    QtMsgType::QtInfoMsg,
                    "php package could not be found, please install it with \"apt get install php\"");
        return -1;
    }

    QString result = QString(process.readAllStandardOutput());
    if(!result.contains("PHP Version"))
    {
        Logger::getInstance().Log(QtMsgType::QtFatalMsg,
                                  "Error while checking php version.");
        return -1;
    }

    int indexBegin = result.indexOf("PHP Version");
    int indexEnd = result.indexOf("\n", indexBegin);
    QString versionString = result.mid(indexBegin, indexEnd - indexBegin);
    Logger::getInstance().Log(QtMsgType::QtInfoMsg, "php package found, details: " + versionString);

    return 0;
}

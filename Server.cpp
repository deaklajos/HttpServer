#include <QNetworkInterface>
#include <QCoreApplication>
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
//    auto ruid = getuid();
//    auto euid = geteuid();
//    Logger::getInstance().Log(QtMsgType::QtFatalMsg, QString("uid: %1, euid: %2").arg(ruid).arg(euid));
//    seteuid(1000);
//    setuid(1000);
//    ruid = getuid();
//    euid = geteuid();
//    Logger::getInstance().Log(QtMsgType::QtFatalMsg, QString("uid: %1, euid: %2").arg(ruid).arg(euid));
//    seteuid(0);
//    setuid(0);
//    ruid = getuid();
//    euid = geteuid();
//    Logger::getInstance().Log(QtMsgType::QtFatalMsg, QString("uid: %1, euid: %2").arg(ruid).arg(euid));
//    return -1;

    // Check for sudo
//    if (geteuid())
//    {
//        Logger::getInstance().Log(QtMsgType::QtFatalMsg, "Start the server with sudo!");
//        return -1;
//    }

//    auto uid = getuid();
//    if (!uid)
//    {
//        Logger::getInstance().Log(
//                    QtMsgType::QtFatalMsg,
//                    "You must start the server as the as the designated user wich sould not be root!");
//        return -1;
//    }

    const qint16 port = 1234;
    if(this->listen(QHostAddress::Any, port))
    {
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

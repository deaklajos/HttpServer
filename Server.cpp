#include <QNetworkInterface>

#include "Server.h"
#include "ResponseWorker.h"
#include "Logger.h"

Server::Server(QObject *parent) :
    QTcpServer(parent)
{
    pool = new QThreadPool(this);
    // TODO use parameter
    pool->setMaxThreadCount(5);

    timer.start();
}

void Server::startServer()
{
    const qint16 port = 1234;
    if(this->listen(QHostAddress::Any, port))
    {

        QString addressString;
        const QHostAddress &localhost = QHostAddress(QHostAddress::LocalHost);
        for (const QHostAddress &address: QNetworkInterface::allAddresses()) {
            if (address.protocol() == QAbstractSocket::IPv4Protocol && address != localhost)
                 addressString += ("," + address.toString() + ":" + QString::number(port));
        }

        addressString.remove(0, 1);
        Logger::getInstance().Log(QtMsgType::QtInfoMsg, "Server started started " + addressString);
    }
    else
    {
        Logger::getInstance().Log(QtMsgType::QtFatalMsg, "Server could not be started. Reason : " + this->errorString());
    }
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
            socket.waitForReadyRead();
            socket.write("HTTP/1.0 503 Service Unavailable\n\n"
                         "<html><h1>503 Service Unavailable</h1>\n"
                         "The server cannot handle the request because it is overloaded.</html>");
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

#include <QNetworkInterface>

#include "Server.h"
#include "ResponseWorker.h"

Server::Server(QObject *parent) :
    QTcpServer(parent)
{
    pool = new QThreadPool(this);
    pool->setMaxThreadCount(5);
}

void Server::startServer()
{
    const qint16 port = 1234;
    if(this->listen(QHostAddress::Any, port))
    {
        qDebug() << "Server started";
        const QHostAddress &localhost = QHostAddress(QHostAddress::LocalHost);
        for (const QHostAddress &address: QNetworkInterface::allAddresses()) {
            if (address.protocol() == QAbstractSocket::IPv4Protocol && address != localhost)
                 qDebug() << address.toString() + ":" + QString::number(port);
        }
    }
    else
    {
        qDebug() << "Server did not start!";
    }
}

void Server::incomingConnection(qintptr handle)
{
    // 1. QTcpServer gets a new connection request from a client.
    // 2. It makes a task (runnable) here.
    // 3. Then, the server grabs one of the threads.
    // 4. The server throws the runnable to the thread.

    // Note: Rannable is a task not a thread
    ResponseWorker *task = new ResponseWorker();
    task->setAutoDelete(true);

    task->socketDescriptor = handle;

    pool->start(task);
    qDebug() << "pool started";
}

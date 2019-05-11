#include <QHostAddress>
#include <QTcpSocket>

#include "ResponseWorker.h"
#include "HttpResponse.h"
#include "Logger.h"

ResponseWorker::ResponseWorker()
{
}

void ResponseWorker::run()
{
    if(!socketDescriptor) return;

    QTcpSocket socket;
    socket.setSocketDescriptor(socketDescriptor);
    QString address = socket.peerAddress().toString();
    QString port = QString::number(socket.peerPort());
    Logger::getInstance().Log(QtMsgType::QtInfoMsg, "Connected to " + address + ":" + port);

    if(socket.waitForReadyRead(500))
    {
        auto response = HttpResponse::fromRequest(socket.readAll());

        socket.write(response.getByteArray());
        socket.flush();
        socket.waitForBytesWritten();
    }
    socket.close();

    Logger::getInstance().Log(QtMsgType::QtInfoMsg, "Closed connection " + address + ":" + port);
}

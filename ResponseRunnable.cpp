#include <QHostAddress>
#include <QTcpSocket>

#include "ResponseRunnable.h"
#include "HttpResponse.h"
#include "Logger.h"

ResponseRunnable::ResponseRunnable()
{
}

void ResponseRunnable::run()
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

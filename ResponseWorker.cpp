#include <QFile>

#include "ResponseWorker.h"
#include "HttpResponse.h"

ResponseWorker::ResponseWorker()
{
}

void ResponseWorker::run()
{
    if(!socketDescriptor) return;

    QTcpSocket socket;
    socket.setSocketDescriptor(socketDescriptor);
    if(socket.waitForReadyRead(500))
    {
        auto response = HttpResponse::fromRequest(socket.readAll());

        socket.write(response.getByteArray());
        socket.flush();
        socket.waitForBytesWritten();
    }
    socket.close();
}

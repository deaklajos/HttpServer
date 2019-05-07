#include <QFile>

#include "ResponseWorker.h"

ResponseWorker::ResponseWorker()
{
}

void ResponseWorker::run()
{
    if(!socketDescriptor) return;

    QString fileName;
    QTcpSocket socket;
    socket.setSocketDescriptor(socketDescriptor);
    if(socket.waitForReadyRead(500))
    {
        QString request(socket.readAll());

        // If index.html
        if(request.contains("GET / "))
            fileName = "index.html";
        if(request.contains("GET /w3schools.jpg"))
            fileName = "w3schools.jpg";
    }
    else
    {
        socket.close();
        return;
    }
    QString responseHeader("HTTP/1.0 200 OK\n\n");
    QString errorResponse("HTTP/1.0 404 Not found\n\n<html>404 Not found</html>");
    QByteArray response = responseHeader.toUtf8();

    QFile file(fileName);
    if(file.exists())
    {
        if(file.open(QIODevice::ReadOnly))
        {
            response.append(file.readAll());
        }
        else
        {
            response = errorResponse.toUtf8();
        }
    }
    else
    {
        response = errorResponse.toUtf8();
    }

    socket.write(response);
    socket.flush();
    socket.waitForBytesWritten();
    socket.close();
}

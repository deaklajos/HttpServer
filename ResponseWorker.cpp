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
    }
    else
    {
        socket.close();
        return;
    }

    QString responseHeader("HTTP/1.0 200 OK\n\n");
    QString errorResponse("HTTP/1.0 404 Not found\n\n<html>404 Not found</html>");
    QString response = responseHeader;

    QFile file(fileName);
    if(file.exists())
    {
        if(file.open(QIODevice::ReadOnly))
        {
            QString filestr(file.readAll());
            response += filestr;
        }
        else
        {
            response = errorResponse;
        }
    }
    else
    {
        response = errorResponse;
    }

    socket.write(response.toUtf8());
    socket.flush();
    socket.waitForBytesWritten();
    socket.close();
}

#include <QNetworkInterface>

#include "HttpResponse.h"

#define OK_HEADER "HTTP/1.0 200 OK\n\n"
#define ERROR_NOT_FOUND "HTTP/1.0 404 Not Found\n\n<html>404 Not Found</html>"
#define ERROR_NOT_IMPLEMENTED "HTTP/1.0 501 Not Implemented\n\n<html>501 Not Implemented</html>"
#define ERROR_INTERNAL_SERVER_ERROR "HTTP/1.0 500 Internal Server Error\n\n<html>500 Internal Server Error</html>"

HttpResponse::HttpResponse(QByteArray data): data(data) {}

HttpResponse HttpResponse::fromRequest(QByteArray request)
{
    QString requestString(request);
    if(!requestString.contains("GET"))
        return HttpResponse(QString(ERROR_NOT_IMPLEMENTED).toUtf8());

    QStringList list = requestString.split(QRegExp("\\s+"), QString::SkipEmptyParts);
    QString resourceLocation = list[1].remove(0, 1);
    if(resourceLocation.isEmpty())
        resourceLocation = "index.html";
    QFile file(resourceLocation);
    if(file.exists())
    {
        if(file.open(QIODevice::ReadOnly))
        {
            QByteArray response = QString(OK_HEADER).toUtf8();
            response.append(file.readAll());
            return HttpResponse(response);
        }
        else
            return HttpResponse(QString(ERROR_INTERNAL_SERVER_ERROR).toUtf8());
    }
    else
        return HttpResponse(QString(ERROR_NOT_FOUND).toUtf8());
}

QByteArray HttpResponse::getByteArray()
{
    return data;
}

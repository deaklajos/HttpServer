#include <QNetworkInterface>

#include "HttpResponse.h"
#include "Logger.h"

#define OK_HEADER "HTTP/1.0 200 OK\nContent-Length: %1\n\n"

#define BAD_REQUEST_HEADER "HTTP/1.0 400 Bad Request\nContent-Length: %1\n\n"
#define NOT_FOUND_HEADER "HTTP/1.0 404 Not Found\nContent-Length: %1\n\n"
#define INTERNAL_SERVER_ERROR_HEADER "HTTP/1.0 500 Internal Server Error\nContent-Length: %1\n\n"
#define NOT_IMPLEMENTED_HEADER "HTTP/1.0 501 Not Implemented\nContent-Length: %1\n\n"
#define SERVICE_UNAVAILABLE_HEADER "HTTP/1.0 503 Service Unavailable\nContent-Length: %1\n\n"

#define BAD_REQUEST_BODY "<html><h1>400 Bad Request</h1>The server cannot process the request due to invalid request.</html>"
#define NOT_FOUND_BODY "<html><h1>404 Not Found</h1>The requested resource could not be found but may be available in the future.</html>"
#define INTERNAL_SERVER_ERROR_BODY "<html><h1>500 Internal Server Error</h1>Unexpected condition encountered.</html>"
#define NOT_IMPLEMENTED_BODY "<html><h1>501 Not Implemented</h1>The server does not recognize the request method.</html>"
#define SERVICE_UNAVAILABLE_BODY "<html><h1>503 Service Unavailable</h1>The server cannot handle the request because it is overloaded.</html>"

HttpResponse::HttpResponse(const QByteArray& data): data(data) {}

HttpResponse HttpResponse::fromRequest(const QByteArray& request)
{
    QString requestString(request);
    QStringList list = requestString.split(QRegExp("\\s+"), QString::SkipEmptyParts);

    if(list.count() < 2 || list[1].length() < 1)
    {
        Logger::getInstance().Log(QtMsgType::QtWarningMsg, "Bad request encountered.");
        return badRequest(true);
    }

    bool isHead = QString("HEAD") == list[0];
    if(QString("GET") != list[0] && !isHead)
    {
        Logger::getInstance().Log(QtMsgType::QtWarningMsg, "Non implemented request encountered.");
        return notImplemented(true);
    }

    // TODO maybe not essential after chroot
    QString resourceLocation = list[1].remove(0, 1);

    if(resourceLocation.isEmpty())
        resourceLocation = "index.html";

    if(resourceLocation.endsWith(".log"), Qt::CaseInsensitive)
    {
        resourceLocation = "";
        Logger::getInstance().Log(QtMsgType::QtCriticalMsg, "Log file requested!");
    }
    else
        Logger::getInstance().Log(QtMsgType::QtInfoMsg, "Requested resource: " + resourceLocation);

    QFile file(resourceLocation);
    if(file.exists())
    {
        if(file.open(QIODevice::ReadOnly))
        {
            QByteArray response;
            if(isHead)
                response = QString(OK_HEADER).arg(file.size()).toUtf8();
            else
            {
                QByteArray content = file.readAll();
                response = QString(OK_HEADER).arg(content.size()).toUtf8();
                response.append(content);
            }

            return HttpResponse(response);
        }
        else
        {
            Logger::getInstance().Log(QtMsgType::QtCriticalMsg, "Could not open resource.");
            return internalServerError(!isHead);
        }

    }
    else
    {
        Logger::getInstance().Log(QtMsgType::QtWarningMsg, "Resource could not be found.");
        return notFound(!isHead);
    }
}

HttpResponse HttpResponse::badRequest(bool withBody)
{
    QString response;
    if(withBody)
        response = QString(
                    BAD_REQUEST_HEADER
                    BAD_REQUEST_BODY
                    ).arg(strlen(BAD_REQUEST_BODY));
    else
        response = QString(
                    BAD_REQUEST_HEADER
                    ).arg(0);

    Logger::getInstance().Log(QtMsgType::QtWarningMsg, "Bad request encountered.");
    return HttpResponse(response.toUtf8());
}

HttpResponse HttpResponse::notFound(bool withBody)
{
    QString response;
    if(withBody)
        response = QString(
                    NOT_FOUND_HEADER
                    NOT_FOUND_BODY
                    ).arg(strlen(NOT_FOUND_BODY));
    else
        response = QString(
                    NOT_FOUND_HEADER
                    ).arg(0);

    return HttpResponse(response.toUtf8());
}

HttpResponse HttpResponse::internalServerError(bool withBody)
{
    QString response;
    if(withBody)
        response = QString(
                    INTERNAL_SERVER_ERROR_HEADER
                    INTERNAL_SERVER_ERROR_BODY
                    ).arg(strlen(INTERNAL_SERVER_ERROR_BODY));
    else
        response = QString(
                    INTERNAL_SERVER_ERROR_HEADER
                    ).arg(0);

    return HttpResponse(response.toUtf8());
}

HttpResponse HttpResponse::notImplemented(bool withBody)
{
    QString response;
    if(withBody)
        response = QString(
                    NOT_IMPLEMENTED_HEADER
                    NOT_IMPLEMENTED_BODY
                    ).arg(strlen(NOT_IMPLEMENTED_BODY));

    else
        response = QString(
                    NOT_IMPLEMENTED_HEADER
                    ).arg(0);

    return HttpResponse(response.toUtf8());
}

HttpResponse HttpResponse::serviceUnavailabe(bool withBody)
{
    QString response;
    if(withBody)
        response = QString(
                    SERVICE_UNAVAILABLE_HEADER
                    SERVICE_UNAVAILABLE_BODY
                    ).arg(strlen(SERVICE_UNAVAILABLE_BODY));

    else
        response = QString(
                    SERVICE_UNAVAILABLE_HEADER
                    ).arg(0);

    return HttpResponse(response.toUtf8());
}

const QByteArray& HttpResponse::getByteArray() const
{
    return data;
}

#ifndef HTTPRESPONSE_H
#define HTTPRESPONSE_H

#include <QByteArray>
#include <QFile>

class HttpResponse
{
public:
    static HttpResponse fromRequest(QByteArray request);
    QByteArray getByteArray();

private:
    HttpResponse(QByteArray data);

    QByteArray data;
};

#endif

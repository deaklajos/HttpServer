#ifndef HTTPRESPONSE_H
#define HTTPRESPONSE_H

#include <QByteArray>
#include <QFile>

class HttpResponse
{
public:
    static HttpResponse fromRequest(const QByteArray& request);
    static HttpResponse fromPHP(const QString& scriptURI, const QString& parameterString);
    static HttpResponse badRequest(bool withBody);
    static HttpResponse notFound(bool withBody);
    static HttpResponse internalServerError(bool withBody);
    static HttpResponse notImplemented(bool withBody);
    static HttpResponse serviceUnavailabe(bool withBody);

    const QByteArray& getByteArray() const;

private:
    HttpResponse(const QByteArray& data);

    QByteArray data;
};

#endif

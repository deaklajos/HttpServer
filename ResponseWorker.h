#ifndef RESPONSEWORKER_H
#define RESPONSEWORKER_H

#include <QRunnable>
#include <QTcpSocket>
#include <QDebug>


class ResponseWorker : public QRunnable
{
public:
    ResponseWorker();

protected:
    void run();

public:
    qintptr socketDescriptor;

};

#endif

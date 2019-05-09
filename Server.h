#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QThreadPool>
#include <QDebug>
#include <QElapsedTimer>

class Server : public QTcpServer
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = 0);
    int startServer();

protected:
    void incomingConnection( qintptr handle );

signals:

public slots:

private:
    QThreadPool *pool;
    QElapsedTimer timer;
    qlonglong connectionCounter = 0;
};

#endif
